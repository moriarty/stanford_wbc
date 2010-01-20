/*
 * Stanford Whole-Body Control Framework http://stanford-wbc.sourceforge.net/
 *
 * Copyright (c) 1997-2009 Stanford University. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>
 */

#include "tao_util.hpp"
#include <tao/dynamics/taoDNode.h>
#include <tao/dynamics/taoJoint.h>
#include <wbcnet/strutil.hpp>

namespace wbc {
  
  void mapNodesToIDs(idToNodeMap_t & idToNodeMap,
		     taoDNode * node)
    throw(std::runtime_error)
  {
    deInt id = node->getID();
    if (idToNodeMap.find( id ) != idToNodeMap.end())
      throw std::runtime_error("wbc::mapNodesToIDs(): duplicate ID " + sfl::to_string(id));
    idToNodeMap.insert(std::make_pair(id, node));
    
    // recurse
    for( taoDNode* p = node->getDChild(); p != NULL; p = p->getDSibling() )
      mapNodesToIDs(idToNodeMap, p);
  }
  
  
  int countNumberOfLinks(taoDNode * root)
  {
    int count(0);
    for (taoDNode * child(root->getDChild()); child != NULL; child = child->getDSibling()) {
      ++count;
      count += countNumberOfLinks(child);
    }
    return count;
  }
  
  
  int countNumberOfJoints(taoDNode * node)
  {
    int count(0);
    for (taoJoint * joint(node->getJointList()); 0 != joint; joint = joint->getNext()) {
      ++count;
    }
    for (taoDNode * child(node->getDChild()); 0 != child; child = child->getDSibling()) {
      count += countNumberOfJoints(child);
    }
    return count;
  }
  
  
  int countDegreesOfFreedom(taoDNode * node)
  {
    int dof(0);
    for (taoJoint * joint(node->getJointList()); 0 != joint; joint = joint->getNext()) {
      dof += joint->getDOF();
    }
    for (taoDNode * child(node->getDChild()); 0 != child; child = child->getDSibling()) {
      dof += countDegreesOfFreedom(child);
    }
    return dof;
  }
  
  
  double computeTotalMass(taoDNode * node)
  {
    double mass(0);
    if (node->mass()) {
      // I guess TAO nodes always have a mass, but the interface
      // returns a pointer, so maybe there are cases where there is
      // not even a zero mass? Whatever, just be paranoid and check
      // for non-NULL pointers.
      mass = *node->mass();
    }
    for (taoDNode * child(node->getDChild()); child != NULL; child = child->getDSibling()) {
      mass += computeTotalMass(child);
    }
    return mass;
  }

}
