/*
 * Stanford Whole-Body Control Framework http://stanford-wbc.sourceforge.net/
 *
 * Copyright (c) 2010 Stanford University. All rights reserved.
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

/**
   \file model_library.cpp
   \author Roland Philippsen
*/

#include "model_library.hpp"
#include "util.hpp"
#include "sai_brep.hpp"
#include "sai_brep_parser.hpp"
#include <string.h>
#include <errno.h>
#include <stdlib.h>

using namespace std;

namespace jspace {
  namespace test {


    static std::string create_puma_xml() throw(runtime_error)
    {
      static char const * xml = 
	"<?xml version=\"1.0\" ?>\n"
	"<dynworld>\n"
	"  <baseNode>\n"
	"    <robotName>Puma</robotName>\n"
	"    <gravity>0, 0, -9.81</gravity>\n"
	"    <pos>0, 0, 0</pos>\n"
	"    <rot>1, 0, 0, 0</rot>\n"
	"    <name>base</name>\n"
	"    <ID>-1</ID>\n"
	"    <jointNode>\n"
	"      <jointName>shoulder-yaw</jointName>\n"
	"      <linkName>base</linkName>\n"
	"      <upperJointLimit>0.1</upperJointLimit>\n"
	"      <lowerJointLimit>-0.1</lowerJointLimit>\n"
	"      <defaultJointPosition>0</defaultJointPosition>\n"
	"      <type>R</type>\n"
	"      <axis>Z</axis>\n"
	"      <mass>34.40</mass>\n"
	"      <inertia>0.00, 0.00, 1.49</inertia>\n"
	"      <com>0, 0, 0</com>\n"
	"      <pos>0, 0, 0</pos>\n"
	"      <rot>0, 0, 1, 0</rot>\n"
	"      <name>NoName</name>\n"
	"      <ID>0</ID>\n"
	"      <jointNode>\n"
	"        <jointName>shoulder-pitch</jointName>\n"
	"        <linkName>upper_arm</linkName>\n"
	"        <upperJointLimit>0.1</upperJointLimit>\n"
	"        <lowerJointLimit>-0.1</lowerJointLimit>\n"
	"        <defaultJointPosition>0</defaultJointPosition>\n"
	"        <type>R</type>\n"
	"        <axis>Z</axis>\n"
	"        <mass>17.40</mass>\n"
	"        <inertia>0.13, 0.524, 5.249</inertia>\n"
	"        <com>0.068, 0.006, -0.016</com>\n"
	"        <pos>0.0, 0.2435, 0.0</pos>\n"
	"        <rot>1, 0, 0, -1.57079632679489661923</rot>\n"
	"        <name>upper_arm</name>\n"
	"        <ID>1</ID>\n"
	"        <jointNode>\n"
	"          <jointName>elbow</jointName>\n"
	"          <linkName>lower_arm</linkName>\n"
	"          <upperJointLimit>0.1</upperJointLimit>\n"
	"          <lowerJointLimit>-0.1</lowerJointLimit>\n"
	"          <defaultJointPosition>0</defaultJointPosition>\n"
	"          <type>R</type>\n"
	"          <axis>Z</axis>\n"
	"          <mass>6.04</mass>\n"
	"          <inertia>0.192, 0.0154, 1.042</inertia>\n"
	"          <com>0, -0.143, 0.014</com>\n"
	"          <pos>0.4318, 0, -0.0934</pos>\n"
	"          <rot>1, 0, 0, 0</rot>\n"
	"          <name>lower_arm</name>\n"
	"          <ID>2</ID>\n"
	"          <jointNode>\n"
	"            <jointName>wrist-roll1</jointName>\n"
	"            <linkName>wrist-hand</linkName>\n"
	"            <upperJointLimit>0.1</upperJointLimit>\n"
	"            <lowerJointLimit>-0.1</lowerJointLimit>\n"
	"            <defaultJointPosition>0</defaultJointPosition>\n"
	"            <type>R</type>\n"
	"            <axis>Z</axis>\n"
	"            <mass>0.82</mass>\n"
	"            <inertia>0.0018, 0.0018, 0.2013</inertia>\n"
	"            <com>0.0, 0.0, -0.019</com>\n"
	"            <pos>-0.0203, -0.4331, 0.0</pos>\n"
	"            <rot>1, 0, 0, 1.57079632679489661923</rot>\n"
	"            <name>wrist-hand</name>\n"
	"            <ID>3</ID>\n"
	"            <jointNode>\n"
	"              <jointName>wrist-pitch</jointName>\n"
	"              <linkName>wrist-finger</linkName>\n"
	"              <upperJointLimit>0.1</upperJointLimit>\n"
	"              <lowerJointLimit>-0.1</lowerJointLimit>\n"
	"              <defaultJointPosition>0</defaultJointPosition>\n"
	"              <type>R</type>\n"
	"              <axis>Z</axis>\n"
	"              <mass>0.34</mass>\n"
	"              <inertia>0.0003, 0.0003, 0.1794</inertia>\n"
	"              <com>0.0, 0.0, 0.0</com>\n"
	"              <pos>0, 0, 0</pos>\n"
	"              <rot>1, 0, 0, -1.57079632679489661923</rot>\n"
	"              <name>wrist-finger</name>\n"
	"              <ID>4</ID>\n"
	"              <jointNode>\n"
	"                <jointName>wrist-roll2</jointName>\n"
	"                <linkName>end-effector</linkName>\n"
	"                <upperJointLimit>0.1</upperJointLimit>\n"
	"                <lowerJointLimit>-0.1</lowerJointLimit>\n"
	"                <defaultJointPosition>0</defaultJointPosition>\n"
	"                <type>R</type>\n"
	"                <axis>Z</axis>\n"
	"                <mass>0.09</mass>\n"
	"                <inertia>0.00015, 0.00015, 0.19304</inertia>\n"
	"                <com>0.0, 0.0, 0.032</com>\n"
	"                <pos>0, 0, 0</pos>\n"
	"                <rot>1, 0, 0, 1.57079632679489661923</rot>\n"
	"                <name>end-effector</name>\n"
	"                <ID>5</ID>\n"
	"              </jointNode>\n"
	"            </jointNode>\n"
	"          </jointNode>\n"
	"        </jointNode>\n"
	"      </jointNode>\n"
	"    </jointNode>\n"
	"  </baseNode>\n"
	"</dynworld>\n";
      std::string result(create_tmpfile("puma.xml.XXXXXX", xml));
      return result;
    }


    static BranchingRepresentation * create_puma_brep() throw(runtime_error)
    {
      static string xml_filename("");
      if (xml_filename.empty()) {
	xml_filename = create_puma_xml();
      }
      BRParser brp;
      BranchingRepresentation * brep(brp.parse(xml_filename));
      return brep;
    }

    
    jspace::Model * create_puma_model() throw(runtime_error)
    {
      BranchingRepresentation * kg_brep(create_puma_brep());
      jspace::tao_tree_info_s * kg_tree(kg_brep->createTreeInfo());
      delete kg_brep;
      
      BranchingRepresentation * cc_brep(create_puma_brep());
      jspace::tao_tree_info_s * cc_tree(cc_brep->createTreeInfo());
      delete cc_brep;
      
      jspace::Model * model(new jspace::Model(kg_tree, cc_tree));
      return model;
    }
    

    static std::string create_unit_mass_RR_xml() throw(runtime_error)
    {
      static char const * xml = 
	"<?xml version=\"1.0\" ?>\n"
	"<dynworld>\n"
	"  <baseNode>\n"
	"    <gravity>0, 0, -9.81</gravity>\n"
	"    <pos>0, 0, 0</pos>\n"
	"    <rot>1, 0, 0, 0</rot>\n"
	"    <jointNode>\n"
	"      <ID>0</ID>\n"
	"      <type>R</type>\n"
	"      <axis>X</axis>\n"
	"      <mass>1</mass>\n"
	"      <inertia>0, 0, 0</inertia>\n"
	"      <com>0, 1, 0</com>\n"
	"      <pos>0, 0, 0</pos>\n"
	"      <rot>0, 0, 1, 0</rot>\n"
	"      <jointNode>\n"
	"        <ID>1</ID>\n"
	"        <type>R</type>\n"
	"        <axis>X</axis>\n"
	"        <mass>1</mass>\n"
	"        <inertia>0, 0, 0</inertia>\n"
	"        <com>0, 1, 0</com>\n"
	"        <pos>0, 1, 0</pos>\n"
	"        <rot>0, 0, 1, 0</rot>\n"
	"      </jointNode>\n"
	"    </jointNode>\n"
	"  </baseNode>\n"
	"</dynworld>\n";
      std::string result(create_tmpfile("unit_mass_RR.xml.XXXXXX", xml));
      return result;
    }


    static BranchingRepresentation * create_unit_mass_RR_brep() throw(runtime_error)
    {
      static string xml_filename("");
      if (xml_filename.empty()) {
	xml_filename = create_unit_mass_RR_xml();
      }
      BRParser brp;
      BranchingRepresentation * brep(brp.parse(xml_filename));
      return brep;
    }


    jspace::Model * create_unit_mass_RR_model() throw(runtime_error)
    {
      BranchingRepresentation * kg_brep(create_unit_mass_RR_brep());
      jspace::tao_tree_info_s * kg_tree(kg_brep->createTreeInfo());
      delete kg_brep;
      
      BranchingRepresentation * cc_brep(create_unit_mass_RR_brep());
      jspace::tao_tree_info_s * cc_tree(cc_brep->createTreeInfo());
      delete cc_brep;
      
      jspace::Model * model(new jspace::Model(kg_tree, cc_tree));
      return model;
    }


    static std::string create_unit_mass_5R_xml() throw(runtime_error)
    {
      static char const * xml = 
	"<?xml version=\"1.0\" ?>\n"
	"<dynworld>\n"
	"  <baseNode>\n"
	"    <gravity>0, 0, -9.81</gravity>\n"
	"    <pos>0, 0, 0</pos>\n"
	"    <rot>1, 0, 0, 0</rot>\n"
	"    <jointNode>\n"
	"      <ID>0</ID>\n"
	"      <type>R</type>\n"
	"      <axis>Z</axis>\n"
	"      <mass>1</mass>\n"
	"      <inertia>0, 0, 0</inertia>\n"
	"      <com>1, 0, 0</com>\n"
	"      <pos>0, 0, 2</pos>\n"
	"      <rot>1, 0, 0, 0</rot>\n"
	"      <jointNode>\n"
	"        <ID>1</ID>\n"
	"        <type>R</type>\n"
	"        <axis>Z</axis>\n"
	"        <mass>1</mass>\n"
	"        <inertia>0, 0, 0</inertia>\n"
	"        <com>1, 0, 0</com>\n"
	"        <pos>1, 0, 0</pos>\n"
	"        <rot>1, 0, 0, 0</rot>\n"
	"        <jointNode>\n"
	"          <ID>2</ID>\n"
	"          <type>R</type>\n"
	"          <axis>Z</axis>\n"
	"          <mass>1</mass>\n"
	"          <inertia>0, 0, 0</inertia>\n"
	"          <com>1, 0, 0</com>\n"
	"          <pos>1, 0, 0</pos>\n"
	"          <rot>1, 0, 0, 0</rot>\n"
	"          <jointNode>\n"
	"            <ID>3</ID>\n"
	"            <type>R</type>\n"
	"            <axis>Z</axis>\n"
	"            <mass>1</mass>\n"
	"            <inertia>0, 0, 0</inertia>\n"
	"            <com>1, 0, 0</com>\n"
	"            <pos>1, 0, 0</pos>\n"
	"            <rot>1, 0, 0, 0</rot>\n"
	"            <jointNode>\n"
	"              <ID>4</ID>\n"
	"              <type>R</type>\n"
	"              <axis>Z</axis>\n"
	"              <mass>1</mass>\n"
	"              <inertia>0, 0, 0</inertia>\n"
	"              <com>1, 0, 0</com>\n"
	"              <pos>1, 0, 0</pos>\n"
	"              <rot>1, 0, 0, 0</rot>\n"
	"            </jointNode>\n"
	"          </jointNode>\n"
	"        </jointNode>\n"
	"      </jointNode>\n"
	"    </jointNode>\n"
	"  </baseNode>\n"
	"</dynworld>\n";
      std::string result(create_tmpfile("unit_mass_5R.xml.XXXXXX", xml));
      return result;
    }


    BranchingRepresentation * create_unit_mass_5R_brep() throw(runtime_error)
    {
      static string xml_filename("");
      if (xml_filename.empty()) {
	xml_filename = create_unit_mass_5R_xml();
      }
      BRParser brp;
      BranchingRepresentation * brep(brp.parse(xml_filename));
      return brep;
    }
    
    
    jspace::Model * create_unit_mass_5R_model() throw(runtime_error)
    {
      BranchingRepresentation * kg_brep(create_unit_mass_5R_brep());
      jspace::tao_tree_info_s * kg_tree(kg_brep->createTreeInfo());
      delete kg_brep;
      
      BranchingRepresentation * cc_brep(create_unit_mass_5R_brep());
      jspace::tao_tree_info_s * cc_tree(cc_brep->createTreeInfo());
      delete cc_brep;
      
      jspace::Model * model(new jspace::Model(kg_tree, cc_tree));
      return model;
    }


    static std::string create_unit_inertia_RR_xml() throw(runtime_error)
    {
      static char const * xml = 
	"<?xml version=\"1.0\" ?>\n"
	"<dynworld>\n"
	"  <baseNode>\n"
	"    <gravity>0, 0, -9.81</gravity>\n"
	"    <pos>0, 0, 0</pos>\n"
	"    <rot>1, 0, 0, 0</rot>\n"
	"    <jointNode>\n"
	"      <ID>0</ID>\n"
	"      <type>R</type>\n"
	"      <axis>X</axis>\n"
	"      <mass>1</mass>\n"
	"      <inertia>1, 1, 1</inertia>\n"
	"      <com>0, 1, 0</com>\n"
	"      <pos>0, 0, 0</pos>\n"
	"      <rot>0, 0, 1, 0</rot>\n"
	"      <jointNode>\n"
	"        <ID>1</ID>\n"
	"        <type>R</type>\n"
	"        <axis>X</axis>\n"
	"        <mass>1</mass>\n"
	"        <inertia>1, 1, 1</inertia>\n"
	"        <com>0, 1, 0</com>\n"
	"        <pos>0, 2, 0</pos>\n"
	"        <rot>0, 0, 1, 0</rot>\n"
	"      </jointNode>\n"
	"    </jointNode>\n"
	"  </baseNode>\n"
	"</dynworld>\n";
      std::string result(create_tmpfile("unit_inertia_RR.xml.XXXXXX", xml));
      return result;
    }


    static BranchingRepresentation * create_unit_inertia_RR_brep() throw(runtime_error)
    {
      static string xml_filename("");
      if (xml_filename.empty()) {
	xml_filename = create_unit_inertia_RR_xml();
      }
      BRParser brp;
      BranchingRepresentation * brep(brp.parse(xml_filename));
      return brep;
    }

    
    jspace::Model * create_unit_inertia_RR_model() throw(runtime_error)
    {
      BranchingRepresentation * kg_brep(create_unit_inertia_RR_brep());
      jspace::tao_tree_info_s * kg_tree(kg_brep->createTreeInfo());
      delete kg_brep;
      
      BranchingRepresentation * cc_brep(create_unit_inertia_RR_brep());
      jspace::tao_tree_info_s * cc_tree(cc_brep->createTreeInfo());
      delete cc_brep;
      
      jspace::Model * model(new jspace::Model(kg_tree, cc_tree));
      return model;
    }


    static std::string create_unit_mass_RP_xml() throw(runtime_error)
    {
      static char const * xml = 
	"<?xml version=\"1.0\" ?>\n"
	"<dynworld>\n"
	"  <baseNode>\n"
	"    <gravity>0, 0, -9.81</gravity>\n"
	"    <pos>0, 0, 0</pos>\n"
	"    <rot>1, 0, 0, 0</rot>\n"
	"    <jointNode>\n"
	"      <ID>0</ID>\n"
	"      <type>R</type>\n"
	"      <axis>X</axis>\n"
	"      <mass>1</mass>\n"
	"      <inertia>0, 0, 0</inertia>\n"
	"      <com>0, 1, 0</com>\n"
	"      <pos>0, 0, 0</pos>\n"
	"      <rot>0, 0, 1, 0</rot>\n"
	"      <jointNode>\n"
	"        <ID>1</ID>\n"
	"        <type>P</type>\n"
	"        <axis>Z</axis>\n"
	"        <mass>1</mass>\n"
	"        <inertia>0, 0, 0</inertia>\n"
	"        <com>0, 0, 0</com>\n"
	"        <pos>0, 1, 0</pos>\n"
	"        <rot>0, 0, 1, 0</rot>\n"
	"      </jointNode>\n"
	"    </jointNode>\n"
	"  </baseNode>\n"
	"</dynworld>\n";
      std::string result(create_tmpfile("unit_mass_RP.xml.XXXXXX", xml));
      return result;
    }


    static BranchingRepresentation * create_unit_mass_RP_brep() throw(runtime_error)
    {
      static string xml_filename("");
      if (xml_filename.empty()) {
	xml_filename = create_unit_mass_RP_xml();
      }
      BRParser brp;
      BranchingRepresentation * brep(brp.parse(xml_filename));
      return brep;
    }

    
    jspace::Model * create_unit_mass_RP_model() throw(runtime_error)
    {
      BranchingRepresentation * kg_brep(create_unit_mass_RP_brep());
      jspace::tao_tree_info_s * kg_tree(kg_brep->createTreeInfo());
      delete kg_brep;
      
      BranchingRepresentation * cc_brep(create_unit_mass_RP_brep());
      jspace::tao_tree_info_s * cc_tree(cc_brep->createTreeInfo());
      delete cc_brep;
      
      jspace::Model * model(new jspace::Model(kg_tree, cc_tree));
      return model;
    }


    void compute_unit_mass_RR_mass_inertia(double q1, double q2, jspace::Matrix & AA)
    {
      double const c1(cos(q1));
      double const c12(cos(q1+q2));
      double const s1(sin(q1));
      double const s12(sin(q1+q2));
      AA.resize(2, 2);
      AA.coeffRef(0, 0) = 1 + pow(s1 + s12, 2) + pow(c1 + c12, 2);
      AA.coeffRef(1, 0) = s12 * (s1 + s12) + c12 * (c1 + c12);
      AA.coeffRef(0, 1) = AA.coeff(1, 0);
      AA.coeffRef(1, 1) = 1;
    }


    void compute_unit_inertia_RR_mass_inertia(double q1, double q2, jspace::Matrix & AA)
    {
      double const c1(cos(q1));
      double const c12(cos(q1+q2));
      double const s1(sin(q1));
      double const s12(sin(q1+q2));
      AA.resize(2, 2);
      AA.coeffRef(0, 0) = 3 + pow(2*s1 + s12, 2) + pow(2*c1 + c12, 2);
      AA.coeffRef(1, 0) = 1 + s12 * (2*s1 + s12) + c12 * (2*c1 + c12);
      AA.coeffRef(0, 1) = AA.coeff(1, 0);
      AA.coeffRef(1, 1) = 2;
    }

  }
}
