#include "pyre.h"

using cyclus::Material;
using cyclus::Composition;
using cyclus::toolkit::ResBuf;
using cyclus::toolkit::MatVec;
using cyclus::KeyError;
using cyclus::ValueError;
using cyclus::Request;
using cyclus::CompMap;

typedef std::pair<double, std::map<int, double> > Stream;
typedef std::map<std::string, Stream> StreamSet;

namespace recycle {

Pyre::Pyre(cyclus::Context* ctx) 
    : cyclus::Facility(ctx),
      latitude(0.0),
      longitude(0.0),
      coordinates(latitude, longitude) {
      }

cyclus::Inventories Pyre::SnapshotInv() {
  cyclus::Inventories invs;  

  // these inventory names are intentionally convoluted so as to not clash
  // with the user-specified stream commods that are used as the separations
  // streams inventory names.
  invs["leftover-inv-name"] = leftover.PopNRes(leftover.count());
  leftover.Push(invs["leftover-inv-name"]);
  invs["feed-inv-name"] = feed.PopNRes(feed.count());
  feed.Push(invs["feed-inv-name"]);

  std::map<std::string, ResBuf<Material> >::iterator it;
  for (it = streambufs.begin(); it != streambufs.end(); ++it) {
    invs[it->first] = it->second.PopNRes(it->second.count());
    it->second.Push(invs[it->first]);
  }

  return invs;
}

void Pyre::InitInv(cyclus::Inventories& inv) {
  leftover.Push(inv["leftover-inv-name"]);
  feed.Push(inv["feed-inv-name"]);

  cyclus::Inventories::iterator it;
  for (it = inv.begin(); it != inv.end(); ++it) {
    streambufs[it->first].Push(it->second);
  }
}

void Pyre::EnterNotify() {
  cyclus::Facility::EnterNotify();
  std::map<int, double> efficiency_;
  efficiency_ = AddEffs();

  std::map<int, double>::iterator effs;
  std::vector<int> eff_pb_;
  for (effs = efficiency_.begin(); effs != efficiency_.end(); effs++) {
    if (effs->second > 1) {
      eff_pb_.push_back(effs->first);
    }
  }

  if (eff_pb_.size() > 0) {
    std::stringstream ss;
    ss << "In " << prototype() << ", ";
    ss << "the following nuclide(s) have a cumulative separation efficiency "
          "greater than 1:";
    for (int i = 0; i < eff_pb_.size(); i++) {
      ss << "\n    " << eff_pb_[i];
      if (i < eff_pb_.size() - 1) {
        ss << ",";
      } else {
        ss << ".";
      }
    }

    throw cyclus::ValueError(ss.str());
  }

  if (feed_commod_prefs.size() == 0) {
    for (int i = 0; i < feed_commods.size(); i++) {
      feed_commod_prefs.push_back(cyclus::kDefaultPref);
    }
  }
}

std::map<int, double> Pyre::AddEffs() {
  StreamSet::iterator i;
  std::map<int, double> eff_map;
  for (i = streams_.begin(); i != streams_.end(); ++i) {
    std::string name = i->first;
    Stream stream = i->second;
    double cap = stream.first;
    if (cap >= 0) {
      streambufs[name].capacity(cap);
    }
    std::map<int, double>::iterator iso_pair;
    for (iso_pair = stream.second.begin(); iso_pair != stream.second.end(); iso_pair++) {
      eff_map[iso_pair->first] += iso_pair->second;
    }
    RecordPosition();
  }
  return eff_map;
}

void Pyre::Tick() {
  SetObj();
  if (feed.count() == 0) {
    return;
  }
  double pop_qty = std::min(throughput, feed.quantity());
  Material::Ptr mat = feed.Pop(pop_qty, cyclus::eps_rsrc());
  double orig_qty = mat->quantity();

  std::map<std::string, Material::Ptr> stagedsep;
  Record("Separating", orig_qty, "UNF");
  
  bool divert = d.Divert(context()->time(),components_);
  stagedsep = Separate(mat);
  if (divert) {
    stagedsep = d.DivertStream(stagedsep);
    Record("Diverted", stagedsep["diverted"]->quantity(), d.locate().first);
  }
  double maxfrac = Buffer(stagedsep, mat);

  if (maxfrac == 1) {
    if (mat->quantity() > 0) {
      // unspecified separations fractions go to leftovers
      leftover.Push(mat);
    }
  } else {  // maxfrac is < 1
    // push back any leftover feed due to separated stream inv size constraints
    feed.Push(mat->ExtractQty((1 - maxfrac) * orig_qty));
    if (mat->quantity() > 0) {
      // unspecified separations fractions go to leftovers
      leftover.Push(mat);
    }
  }
}

void Pyre::SetObj() {
  if(context()->time() == 0) {
    v = pyro::Volox(this->volox_temp, this->volox_time, this->volox_flowrate, 
      this->volox_volume);
    components_["volox"] = &v;

    rd = pyro::Reduct(this->reduct_current, this->reduct_lithium_oxide, 
      this->reduct_volume, this->reduct_time);
    components_["reduct"] = &rd;

    rf = pyro::Refine(this->refine_temp, this->refine_press, this->refine_rotation, 
      this->refine_batch_size, this->refine_time);
    components_["refine"] = &rf;

    w = pyro::Winning(this->winning_current, this->winning_time, this->winning_flowrate, 
      this->winning_volume);
    components_["winning"] = &w;

    d = Diverter(std::make_pair(this->location_sub, this->location_par), 
      this->frequency_, this->siphon_, this->divert_num_, this->type_);
  }
}

std::map<std::string, Material::Ptr> Pyre::Separate(Material::Ptr mat) {
  StreamSet::iterator strm;
  std::map<std::string, Material::Ptr> separating;

  for (strm = streams_.begin(); strm != streams_.end(); ++strm) {
    Stream info = strm->second;
    std::string name = strm->first;
    separating[name] = ProcessSeparate(name, info, mat);
    Record("Staged", separating[name]->quantity(), name);
  }
  return separating;
}

Material::Ptr Pyre::ProcessSeparate(std::string name, Stream stream, 
  Material::Ptr mat) {
  Material::Ptr material;
  std::string short_name;
  //removes anything after the underscore in the stream name so it can process properly
  std::istringstream stream_name(name);
  std::getline(stream_name,short_name,'_');

  pyro::Process * subprocess = components_[short_name];
  return subprocess->SepMaterial(stream.second, mat);
}

double Pyre::Buffer(std::map<std::string, Material::Ptr> stagedsep, 
  Material::Ptr mat) {
  std::map<std::string, Material::Ptr>::iterator sep;
  double maxfrac = 1;

  for (sep = stagedsep.begin(); sep != stagedsep.end(); ++sep) {
    std::string name = sep->first;
    Material::Ptr m = sep->second;

    double frac = streambufs[name].space() / stagedsep[name]->quantity();
    if (frac < maxfrac) {
      maxfrac = frac;
    }

    if (m->quantity() > 0) {
      streambufs[name].Push(
          mat->ExtractComp(m->quantity() * maxfrac, m->comp()));
      Record("Separated", m->quantity() * maxfrac, name);
    }
  }
  return maxfrac;
}

std::set<cyclus::RequestPortfolio<Material>::Ptr>
Pyre::GetMatlRequests() {
  using cyclus::RequestPortfolio;
  std::set<RequestPortfolio<Material>::Ptr> ports;

  int t = context()->time();
  int t_exit = exit_time();
  if (t_exit >= 0 && (feed.quantity() >= (t_exit - t) * throughput)) {
    return ports;  // already have enough feed for remainder of life
  } else if (feed.space() < cyclus::eps_rsrc()) {
    return ports;
  }

  bool exclusive = false;
  RequestPortfolio<Material>::Ptr port(new RequestPortfolio<Material>());

  Material::Ptr m = cyclus::NewBlankMaterial(feed.space());
  if (!feed_recipe.empty()) {
    Composition::Ptr c = context()->GetRecipe(feed_recipe);
    m = Material::CreateUntracked(feed.space(), c);
  }

  std::vector<cyclus::Request<Material>*> reqs;
  for (int i = 0; i < feed_commods.size(); i++) {
    std::string commod = feed_commods[i];
    double pref = feed_commod_prefs[i];
    reqs.push_back(port->AddRequest(m, this, commod, pref, exclusive));
  }
  port->AddMutualReqs(reqs);
  ports.insert(port);

  return ports;
}

void Pyre::GetMatlTrades(
    const std::vector<cyclus::Trade<Material> >& trades,
    std::vector<std::pair<cyclus::Trade<Material>, Material::Ptr> >&
        responses) {
  using cyclus::Trade;

  std::vector<cyclus::Trade<cyclus::Material> >::const_iterator it;
  for (int i = 0; i < trades.size(); i++) {
    std::string commod = trades[i].request->commodity();
    if (commod == leftover_commod) {
      double amt = std::min(leftover.quantity(), trades[i].amt);
      Material::Ptr m = leftover.Pop(amt, cyclus::eps_rsrc());
      responses.push_back(std::make_pair(trades[i], m));
    } else if (streambufs.count(commod) > 0) {
      double amt = std::min(streambufs[commod].quantity(), trades[i].amt);
      Material::Ptr m = streambufs[commod].Pop(amt, cyclus::eps_rsrc());
      responses.push_back(std::make_pair(trades[i], m));
    } else {
      throw ValueError("invalid commodity " + commod +
                       " on trade matched to prototype " + prototype());
    }
  }
}

void Pyre::AcceptMatlTrades(
    const std::vector<std::pair<cyclus::Trade<Material>, Material::Ptr> >&
        responses) {
  std::vector<std::pair<cyclus::Trade<cyclus::Material>,
                        cyclus::Material::Ptr> >::const_iterator trade;

  for (trade = responses.begin(); trade != responses.end(); ++trade) {
    feed.Push(trade->second);
  }
}

std::set<cyclus::BidPortfolio<Material>::Ptr> Pyre::GetMatlBids(
    cyclus::CommodMap<Material>::type& commod_requests) {
  using cyclus::BidPortfolio;

  bool exclusive = false;
  std::set<BidPortfolio<Material>::Ptr> ports;

  // bid streams
  std::map<std::string, ResBuf<Material> >::iterator it;
  for (it = streambufs.begin(); it != streambufs.end(); ++it) {
    std::string commod = it->first;
    std::vector<Request<Material>*>& reqs = commod_requests[commod];
    if (reqs.size() == 0) {
      continue;
    } else if (streambufs[commod].quantity() < cyclus::eps_rsrc()) {
      continue;
    }

    MatVec mats = streambufs[commod].PopN(streambufs[commod].count());
    streambufs[commod].Push(mats);

    BidPortfolio<Material>::Ptr port(new BidPortfolio<Material>());

    for (int j = 0; j < reqs.size(); j++) {
      Request<Material>* req = reqs[j];
      double tot_bid = 0;
      for (int k = 0; k < mats.size(); k++) {
        Material::Ptr m = mats[k];
        tot_bid += m->quantity();

        // this fix the problem of the cyclus exchange manager which crashes
        // when a bid with a quantity <=0 is offered.
        if (m->quantity() > cyclus::eps_rsrc()) {
          port->AddBid(req, m, this, exclusive);
        }

        if (tot_bid >= req->target()->quantity()) {
          break;
        }
      }
    }

    double tot_qty = streambufs[commod].quantity();
    cyclus::CapacityConstraint<Material> cc(tot_qty);
    port->AddConstraint(cc);
    ports.insert(port);
  }

  // bid leftovers
  std::vector<Request<Material>*>& reqs = commod_requests[leftover_commod];
  if (reqs.size() > 0 && leftover.quantity() >= cyclus::eps_rsrc()) {
    MatVec mats = leftover.PopN(leftover.count());
    leftover.Push(mats);

    BidPortfolio<Material>::Ptr port(new BidPortfolio<Material>());

    for (int j = 0; j < reqs.size(); j++) {
      Request<Material>* req = reqs[j];
      double tot_bid = 0;
      for (int k = 0; k < mats.size(); k++) {
        Material::Ptr m = mats[k];
        tot_bid += m->quantity();

        // this fix the problem of the cyclus exchange manager which crashes
        // when a bid with a quantity <=0 is offered.
        if (m->quantity() > cyclus::eps_rsrc()) {
          port->AddBid(req, m, this, exclusive);
        }

        if (tot_bid >= req->target()->quantity()) {
          break;
        }
      }
    }

    cyclus::CapacityConstraint<Material> cc(leftover.quantity());
    port->AddConstraint(cc);
    ports.insert(port);
  }

  return ports;
}

void Pyre::Tock() {}

bool Pyre::CheckDecommissionCondition() {
  if (leftover.count() > 0) {
    return false;
  }

  std::map<std::string, ResBuf<Material> >::iterator it;
  for (it = streambufs.begin(); it != streambufs.end(); ++it) {
    if (it->second.count() > 0) {
      return false;
    }
  }

  return true;
}

void Pyre::RecordPosition() {
  std::string specification = this->spec();
  context()
      ->NewDatum("AgentPosition")
      ->AddVal("Spec", specification)
      ->AddVal("Prototype", this->prototype())
      ->AddVal("AgentId", id())
      ->AddVal("Latitude", latitude)
      ->AddVal("Longitude", longitude)
      ->Record();
}

void Pyre::Record(std::string name, double val, std::string type) {
  context()
      ->NewDatum("SeparationEvents")
      ->AddVal("AgentId", id())
      ->AddVal("Time", context()->time())
      ->AddVal("Event", name)
      ->AddVal("Value", val)
      ->AddVal("Type", type)
      ->Record();
}

void Pyre::RecordStreams() {
  context()
      ->NewDatum("Test")
      ->AddVal("AgentId", id())
      ->AddVal("Time", context()->time())
      ->AddVal("Stream", streams_)
      ->Record();
}

extern "C" cyclus::Agent* ConstructPyre(cyclus::Context* ctx) {
  return new Pyre(ctx);
}

}  // namespace recycle
