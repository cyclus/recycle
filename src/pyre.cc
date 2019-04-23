#include "pyre.h"
#include "cyclus.h"

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

// KDH : perhaps you could track other invs of interest in pyre?
void Pyre::InitInv(cyclus::Inventories& inv) {
  leftover.Push(inv["leftover-inv-name"]);
  feed.Push(inv["feed-inv-name"]);

  cyclus::Inventories::iterator it;
  for (it = inv.begin(); it != inv.end(); ++it) {
    streambufs[it->first].Push(it->second);
  }
}

void Pyre::EnterNotify() {
  // KDH : it's weird to me that volox_temp is available in this scope
  // if it's a private member variable... does the c++ style 
  // guide require a following underscore in the var name?
  // (maybe not, this might just be python)
  // (also, maybe ok if these are public vars)
  // also confused why you don't need this->volox_temp
  v = Volox(volox_temp, volox_time, volox_flowrate, 
    volox_volume);

  components_.push_back(v);

  rd = Reduct(reduct_current, reduct_lithium_oxide, 
    reduct_volume, reduct_time);

  components_.push_back(rd);


  rf = Refine(refine_temp, refine_press, refine_rotation, 
    refine_batch_size, refine_time);

  components_.push_back(rf);

  w = Winning(winning_current, winning_time, winning_flowrate, 
    winning_volume);
  
  components_.push_back(w);

  d = Diversion(divert_prob, divert_num);

  cyclus::Facility::EnterNotify();
  // KDH: figure out if you're going to clean this up
  // should it remain in pyre? 
  // if not, get rid of cruft...
  std::map<int, double> efficiency_;

  StreamSet::iterator it;
  for (it = streams_.begin(); it != streams_.end(); ++it) {
    std::string name = it->first;
    Stream stream = it->second;
    double cap = stream.first;
    if (cap >= 0) {
      streambufs[name].capacity(cap);
    }

    // KDH : perhaps it2 could be it_inner or something... 
    // though i, j, k is fine too.
    // best to give it a physical name... is it a stream?
    // it's an entry in the compmap for the stream maybe? 
    // still a bit unclear what a stream is... tbd.
    std::map<int, double>::iterator iso_pair;
    for (iso_pair = stream.second.begin(); iso_pair != stream.second.end(); iso_pair++) {
      efficiency_[iso_pair->first] += iso_pair->second;
    }
    RecordPosition();
  }

  // KDH :maybe these are priority buffers... do they have anything to do with pyre?
  std::map<int, double>::iterator it2;
  std::vector<int> eff_pb_;
  for (it2 = efficiency_.begin(); it2 != efficiency_.end(); it2++) {
    if (it2->second > 1) {
      eff_pb_.push_back(it2->first);
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

void Pyre::Tick() {
  if (feed.count() == 0) {
    return;
  }
  double pop_qty = std::min(throughput, feed.quantity());
  Material::Ptr mat = feed.Pop(pop_qty, cyclus::eps_rsrc());
  double orig_qty = mat->quantity();

  StreamSet::iterator it;
  double maxfrac = 1;
  std::map<std::string, Material::Ptr> stagedsep;
  Record("Separating", orig_qty, "UNF");
  RecordStreams();
  
  // KDH: it wouldn't hurt for this to be its own function (private).
  for (it = streams_.begin(); it != streams_.end(); ++it) {
    Stream info = it->second;
    std::string name = it->first;
    stagedsep[name] = Separate(name, info, mat);
    Record("Staged", stagedsep[name]->quantity(), name);
    double frac = streambufs[name].space() / stagedsep[name]->quantity();
    if (frac < maxfrac) {
      maxfrac = frac;
    }
  }

  // KDH: this could also be its own function (private)
  std::map<std::string, Material::Ptr>::iterator itf;
  for (itf = stagedsep.begin(); itf != stagedsep.end(); ++itf) {
    std::string name = itf->first;
    Material::Ptr m = itf->second;
    if (m->quantity() > 0) {
      streambufs[name].Push(
          mat->ExtractComp(m->quantity() * maxfrac, m->comp()));
      Record("Separated", m->quantity() * maxfrac, name);
    }
  }

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
 
 // KDH : name this in a way that makes sense for pyre... 
Material::Ptr Pyre::Separate(std::string name, Stream stream, 
  Material::Ptr mat) {
  Material::Ptr material;



  // KDH: "volox" should be saved in a var.
  // even better, perhaps you could hold a 
  // list of the possible things we would want to create... 
  
  // imagine pyre has a diverter object (just one)
  // diverter = Diverter(rate, location, whatever)  
  // maybe this happens one level above, in the tick function
  diverter.divert(components_) 

  // and then in the diverter class
  // there is a function called divert that loops through the components
  // and diverts material from them if it's supposed to,
  // but only at the location where it knows it's supposed to.
  // and then the pyre overlay should direct the facilities 
  // to make any physical changes to their dependent vars 
  // accordingly.  
  // alternatively
  for component in components:
    component.divert()
    // component class owns diversion logic
    //diversion is instantiated when component is initialized.

  if (name.find("volox") != std::string::npos) {
    std::string process = "volox"

    // KDH : this should probably all happen in the volox class...
    // volox knows its own physics.
    // any component should be able to answer two questions;
    // at this temp, time, flowrate, etc... (for whatever vars) how much material are we producing?
    // in order to increase production, what changes about each var?
    double v_temp = v.get_temp();
    double v_time = v.get_time();
    double v_flow = v.get_flowrate();

    v.set_temp(DivertMat(v_temp, process, "temp"));
    v.set_time(DivertMat(v_time, process, "time"));
    v.set_flowrate(DivertMat(v_flow, process, "flowrate"));
    material = v.VoloxSepMaterial(stream.second, mat);

  } else if (name.find("reduct") != std::string::npos) {
    std::string process = "reduct";

    double rd_current = rd.get_current();
    double rd_lithium = rd.get_lithium();
    double rd_time = rd.get_time();

    rd.set_current(DivertMat(rd_current, process, "current"));
    rd.set_lithium(DivertMat(rd_lithium, process, "lithium"));
    rd.set_time(DivertMat(rd_time, process, "time"));
    material = rd.ReductSepMaterial(stream.second, mat);
  } else if (name.find("refine") != std::string::npos) {
    std::string process = "refine";

    double rf_temp = rf.get_temp();
    double rf_press = rf.get_pressure();
    double rf_rotation = rf.get_rotation();
    double rf_size = rf.get_size();
    double rf_time = rf.get_time();

    rf.set_temp(DivertMat(rf_temp, process, "temp"));
    rf.set_pressure(DivertMat(rf_press, process, "pressure"));
    rf.set_rotation(DivertMat(rf_rotation, process, "rotation"));
    rf.set_size(DivertMat(rf_size, process, "size"));
    rf.set_time(DivertMat(rf_time, process, "time"));
    material = rf.RefineSepMaterial(stream.second, mat);
  } else if (name.find("winning") != std::string::npos) {
    std::string process = "winning";

    double w_current = w.get_current();
    double w_time = w.get_time();
    double w_flow = w.get_flowrate();

    w.set_current(DivertMat(w_current, process, "current"));
    w.set_time(DivertMat(w_time, process, "time"));
    w.set_flowrate(DivertMat(w_flow, process, "flowrate"));
    material = w.WinningSepMaterial(stream.second, mat);
  } else {
    throw ValueError("Stream names must include the name of the subprocess");
  }
  return material;
}


// KDH this definitely should be implemented in the diversion class.
double Pyre::DivertMat(double input, std::string process, 
  std::string parameter) {
  double divert_prob = d.get_prob();
  int divert_num = d.get_num();
  int times_diverted = d.get_times_divert();

  if (d.divert(divert_prob, divert_num, times_diverted)) {
    times_diverted = times_diverted + 1;
    d.set_times_divert(times_diverted);
    double divert_quant = d.rng_gen(0,5)/100 + 1;
    double divert = input * divert_quant;
    Record("Diverted", divert, parameter);
    return divert;
  } else {
    return input;
  }
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
