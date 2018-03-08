///////////////////////////////////////////////////////////////////////////////
// File: DDHGCalHEAlgo.cc
// Description: Geometry factory class for HGCal (Mix)
///////////////////////////////////////////////////////////////////////////////

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DetectorDescription/Core/interface/DDutils.h"
#include "DetectorDescription/Core/interface/DDSolid.h"
#include "DetectorDescription/Core/interface/DDCurrentNamespace.h"
#include "DetectorDescription/Core/interface/DDSplit.h"
#include "Geometry/HGCalCommonData/plugins/DDHGCalHEAlgo.h"
#include "CLHEP/Units/GlobalPhysicalConstants.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"

//#define EDM_ML_DEBUG

DDHGCalHEAlgo::DDHGCalHEAlgo() {
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: Creating an instance";
#endif
}

DDHGCalHEAlgo::~DDHGCalHEAlgo() {}

void DDHGCalHEAlgo::initialize(const DDNumericArguments & nArgs,
			       const DDVectorArguments & vArgs,
			       const DDMapArguments & ,
			       const DDStringArguments & sArgs,
			       const DDStringVectorArguments &vsArgs) {

  wafers_       = vsArgs["WaferNames"];
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << wafers_.size() 
				<< " wafers";
  for (unsigned int i=0; i<wafers_.size(); ++i)
    edm::LogVerbatim("HGCalGeom") << "Wafer[" << i << "] " << wafers_[i];
#endif
  materials_    = vsArgs["MaterialNames"];
  names_        = vsArgs["VolumeNames"];
  thick_        = vArgs["Thickness"];
  for (unsigned int i=0; i<materials_.size(); ++i) {
    copyNumber_.emplace_back(1);
  }
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << materials_.size()
				<< " types of volumes";
  for (unsigned int i=0; i<names_.size(); ++i)
    edm::LogVerbatim("HGCalGeom") << "Volume [" << i << "] " << names_[i] 
				  << " of thickness " << thick_[i] 
				  << " filled with " << materials_[i]
				  << " first copy number " << copyNumber_[i];
#endif
  layers_       = dbl_to_int(vArgs["Layers"]);
  layerThick_   = vArgs["LayerThick"];
  rMixLayer_    = vArgs["LayerRmix"];
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "There are " << layers_.size() << " blocks";
  for (unsigned int i=0; i<layers_.size(); ++i)
    edm::LogVerbatim("HGCalGeom") << "Block [" << i << "] of thickness "  
				  << layerThick_[i] << " Rmid " <<rMixLayer_[i]
				  << " with " << layers_[i] << " layers";
#endif
  layerType_    = dbl_to_int(vArgs["LayerType"]);
  layerSense_   = dbl_to_int(vArgs["LayerSense"]);
  firstLayer_   = (int)(nArgs["FirstLayer"]);
  if (firstLayer_ > 0) {
    for (unsigned int i=0; i<layerType_.size(); ++i) {
      if (layerSense_[i] != 0) {
	int ii = layerType_[i];
	copyNumber_[ii] = firstLayer_;
#ifdef EDM_ML_DEBUG
	edm::LogVerbatim("HGCalGeom") << "First copy number for layer type "
				      << i << ":" << ii << " with "
				      << materials_[ii] << " changed to "
				      << copyNumber_[ii];
#endif
	break;
      }
    }
  }
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "There are " << layerType_.size() 
				<< " layers" ;
  for (unsigned int i=0; i<layerType_.size(); ++i)
    edm::LogVerbatim("HGCalGeom") << "Layer [" << i << "] with material type " 
				  << layerType_[i] << " sensitive class " 
				  << layerSense_[i];
#endif
  materialsTop_ = vsArgs["TopMaterialNames"];
  namesTop_     = vsArgs["TopVolumeNames"];
  layerThickTop_= vArgs["TopLayerThickness"];
  layerTypeTop_ = dbl_to_int(vArgs["TopLayerType"]);
  for (unsigned int i=0; i<materialsTop_.size(); ++i) {
    copyNumberTop_.emplace_back(1);
  }
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << materialsTop_.size()
				<< " types of volumes in the top part";
  for (unsigned int i=0; i<materialsTop_.size(); ++i)
    edm::LogVerbatim("HGCalGeom") << "Volume [" << i << "] " << namesTop_[i] 
				  << " of thickness " << layerThickTop_[i] 
				  << " filled with " << materialsTop_[i]
				  << " first copy number " <<copyNumberTop_[i];
  edm::LogVerbatim("HGCalGeom") << "There are " << layerTypeTop_.size() 
				<< " layers in the top part" ;
  for (unsigned int i=0; i<layerTypeTop_.size(); ++i)
    edm::LogVerbatim("HGCalGeom") << "Layer [" << i << "] with material type " 
				  << layerTypeTop_[i];
#endif
  materialsBot_ = vsArgs["BottomMaterialNames"];
  namesBot_     = vsArgs["BottomVolumeNames"];
  layerTypeBot_ = dbl_to_int(vArgs["BottomLayerType"]);
  layerSenseBot_= dbl_to_int(vArgs["BottomLayerSense"]);
  layerThickBot_= vArgs["BottomLayerThickness"];
  for (unsigned int i=0; i<materialsBot_.size(); ++i) {
    copyNumberBot_.emplace_back(1);
  }
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << materialsBot_.size()
				<< " types of volumes in the bottom part";
  for (unsigned int i=0; i<materialsBot_.size(); ++i)
    edm::LogVerbatim("HGCalGeom") << "Volume [" << i << "] " << namesBot_[i] 
				  << " of thickness " << layerThickBot_[i] 
				  << " filled with " << materialsBot_[i]
				  << " first copy number " <<copyNumberBot_[i];
  edm::LogVerbatim("HGCalGeom") << "There are " << layerTypeBot_.size() 
				<< " layers in the top part" ;
  for (unsigned int i=0; i<layerTypeBot_.size(); ++i)
    edm::LogVerbatim("HGCalGeom") << "Layer [" << i << "] with material type " 
				  << layerTypeBot_[i] << " sensitive class "
				  << layerSenseBot_[i];
#endif
  zMinBlock_    = nArgs["zMinBlock"];
  rMaxFine_     = nArgs["rMaxFine"];
  rMinThick_    = nArgs["rMinThick"];
  waferSize_    = nArgs["waferSize"];
  waferSepar_   = nArgs["SensorSeparation"];
  sectors_      = (int)(nArgs["Sectors"]);
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: zStart " << zMinBlock_ 
				<< " rFineCoarse " << rMaxFine_ 
				<< " rMaxThick " << rMinThick_
				<< " wafer width " << waferSize_ 
				<< " separations " << waferSepar_
				<< " sectors " << sectors_;
#endif
  slopeB_       = vArgs["SlopeBottom"];
  slopeT_       = vArgs["SlopeTop"];
  zFront_       = vArgs["ZFront"];
  rMaxFront_    = vArgs["RMaxFront"];
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "Bottom slopes " << slopeB_[0] << ":" 
				<< slopeB_[1] << " and " << slopeT_.size() 
				<< " slopes for top" ;
  for (unsigned int i=0; i<slopeT_.size(); ++i)
    edm::LogVerbatim("HGCalGeom") << "Block [" << i << "] Zmin " << zFront_[i]
				  << " Rmax " << rMaxFront_[i] << " Slope " 
				  << slopeT_[i];
#endif
  nameSpace_    = DDCurrentNamespace::ns();
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: NameSpace " << nameSpace_;
#endif
}

////////////////////////////////////////////////////////////////////
// DDHGCalHEAlgo methods...
////////////////////////////////////////////////////////////////////

void DDHGCalHEAlgo::execute(DDCompactView& cpv) {
  
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "==>> Constructing DDHGCalHEAlgo...";
#endif
  copies_.clear();
  constructLayers (parent(), cpv);
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << copies_.size() 
				<< " different wafer copy numbers";
  int k(0);
  for (std::unordered_set<int>::const_iterator itr=copies_.begin();
       itr != copies_.end(); ++itr,++k) {
    edm::LogVerbatim("HGCalGeom") << "Copy [" << k << "] : " << (*itr);
  }
#endif
  copies_.clear();
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "<<== End of DDHGCalHEAlgo construction...";
#endif
}

void DDHGCalHEAlgo::constructLayers(const DDLogicalPart& module, 
				    DDCompactView& cpv) {
  
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: \t\tInside Layers";
#endif
  double       zi(zMinBlock_);
  int          laymin(0);
  const double tol(0.01);
  for (unsigned int i=0; i<layers_.size(); i++) {
    double  zo     = zi + layerThick_[i];
    double  routF  = rMax(zi);
    int     laymax = laymin+layers_[i];
    double  zz     = zi;
    double  thickTot(0);
    for (int ly=laymin; ly<laymax; ++ly) {
      int     ii     = layerType_[ly];
      int     copy   = copyNumber_[ii];
      double  rinB   = (layerSense_[ly] == 0) ? (zo*slopeB_[0]) :
	(zo*slopeB_[1]);
      zz            += (0.5*thick_[ii]);
      thickTot      += thick_[ii];

      std::string name = "HGCal"+names_[ii]+std::to_string(copy);
#ifdef EDM_ML_DEBUG
      edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: Layer " << ly << ":" 
				    << ii << " Front " << zi << ", " << routF
				    << " Back " << zo << ", " << rinB 
				    << " superlayer thickness "
				    << layerThick_[i];
#endif
      DDName matName(DDSplit(materials_[ii]).first, 
		     DDSplit(materials_[ii]).second);
      DDMaterial matter(matName);
      DDLogicalPart glog;
      if (layerSense_[ly] == 0) {
	double alpha = CLHEP::pi/sectors_;
	double rmax  = routF*cos(alpha) - tol;
	std::vector<double> pgonZ, pgonRin, pgonRout;
	pgonZ.emplace_back(-0.5*thick_[ii]);  pgonZ.emplace_back(0.5*thick_[ii]);
	pgonRin.emplace_back(rinB);           pgonRin.emplace_back(rinB);   
	pgonRout.emplace_back(rmax);          pgonRout.emplace_back(rmax);   
	DDSolid solid = DDSolidFactory::polyhedra(DDName(name, nameSpace_),
						  sectors_,-alpha,CLHEP::twopi,
						  pgonZ, pgonRin, pgonRout);
	glog = DDLogicalPart(solid.ddname(), matter, solid);
#ifdef EDM_ML_DEBUG
	edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << solid.name()
				      << " polyhedra of " << sectors_ 
				      << " sectors covering " 
				      << -alpha/CLHEP::deg << ":" 
				      << (-alpha+CLHEP::twopi)/CLHEP::deg
				      << " with " << pgonZ.size()<<" sections";
	for (unsigned int k=0; k<pgonZ.size(); ++k)
	  edm::LogVerbatim("HGCalGeom") << "[" << k << "] z " << pgonZ[k] 
					<< " R " << pgonRin[k] << ":" 
					<< pgonRout[k];
#endif
      } else {
	DDSolid solid = DDSolidFactory::tubs(DDName(name, nameSpace_), 
					     0.5*thick_[ii], rinB, routF, 0.0,
					     CLHEP::twopi);
	glog = DDLogicalPart(solid.ddname(), matter, solid);
#ifdef EDM_ML_DEBUG
	edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << solid.name()
				      << " Tubs made of " << matName 
				      << " of dimensions " << rinB 
				      << ", " << routF << ", " <<0.5*thick_[ii]
				      << ", 0.0, " << CLHEP::twopi/CLHEP::deg;
	edm::LogVerbatim("HGCalGeom") << "Position in: " << glog.name() 
				      << " number "	<< copy;
#endif
	positionMix(glog, name, copy, thick_[ii], matter, rinB, rMixLayer_[i],
		    routF, cpv);
      }
      DDTranslation r1(0,0,zz);
      DDRotation rot;
      cpv.position(glog, module, copy, r1, rot);
      ++copyNumber_[ii];
#ifdef EDM_ML_DEBUG
      edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << glog.name()
				    << " number " << copy 
				    << " positioned in " << module.name() 
				    << " at " << r1 << " with " << rot;
#endif
      zz += (0.5*thick_[ii]);
    } // End of loop over layers in a block
    zi     = zo;
    laymin = laymax;
    if (fabs(thickTot-layerThick_[i]) < 0.00001) {
    } else if (thickTot > layerThick_[i]) {
      edm::LogError("HGCalGeom") << "Thickness of the partition " 
				 << layerThick_[i] << " is smaller than "
				 << thickTot << ": thickness of all its "
				 << "components **** ERROR ****";
    } else if (thickTot < layerThick_[i]) {
      edm::LogWarning("HGCalGeom") << "Thickness of the partition " 
				   << layerThick_[i] << " does not match with "
				   << thickTot << " of the components";
    }
  }   // End of loop over blocks
}

double DDHGCalHEAlgo::rMax(double z) {
  double r(0);
#ifdef EDM_ML_DEBUG
  unsigned int ik(0);
#endif
  for (unsigned int k=0; k<slopeT_.size(); ++k) {
    if (z < zFront_[k]) break;
    r  = rMaxFront_[k] + (z - zFront_[k]) * slopeT_[k];
#ifdef EDM_ML_DEBUG
    ik = k;
#endif
  }
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: rMax : " << z << ":" << ik 
				<< ":" << r;
#endif
  return r;
}

void DDHGCalHEAlgo::positionMix(const DDLogicalPart& glog, 
				const std::string& nameM, int copyM, 
				double thick, const DDMaterial& matter,
				double rin, double rmid, double rout, 
				DDCompactView& cpv) {
  
  DDLogicalPart glog1;
  DDTranslation tran;
  DDRotation    rot;
  for (unsigned int ly=0; ly<layerTypeTop_.size(); ++ly) {
    int     ii         = layerTypeTop_[ly];
    copyNumberTop_[ii] = copyM;
  }
  for (unsigned int ly=0; ly<layerTypeBot_.size(); ++ly) {
    int     ii         = layerTypeBot_[ly];
    copyNumberBot_[ii] = copyM;
  }
  // Make the top part first
  std::string name = nameM+"Top";
  DDSolid solid = DDSolidFactory::tubs(DDName(name, nameSpace_), 
				       0.5*thick, rmid, rout, 0.0,
				       CLHEP::twopi);
  glog1 = DDLogicalPart(solid.ddname(), matter, solid);
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << solid.name() 
				<< " Tubs made of " << matter.name() 
				<< " of dimensions " << rmid << ", " << rout 
				<< ", " << 0.5*thick << ", 0.0, " 
				<< CLHEP::twopi/CLHEP::deg;
#endif
  cpv.position(glog1, glog, 1, tran, rot);
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << glog1.name() 
				<< " number 1 positioned in " << glog.name() 
				<< " at " << tran << " with " << rot;
#endif
  double thickTot(0), zpos(-0.5*thick);
  for (unsigned int ly=0; ly<layerTypeTop_.size(); ++ly) {
    int     ii     = layerTypeTop_[ly];
    int     copy   = copyNumberTop_[ii];
    thickTot      += layerThickTop_[ii];
    name           = nameM+namesTop_[ii]+std::to_string(copy);    
#ifdef EDM_ML_DEBUG
    edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: Layer " << ly << ":" << ii
				  << " R " << rmid << ":" << rout << " Thick " 
				  << layerThickTop_[ly];
#endif
    DDName matName(DDSplit(materialsTop_[ii]).first, 
		   DDSplit(materialsTop_[ii]).second);
    DDMaterial matter1(matName);
    solid = DDSolidFactory::tubs(DDName(name,nameSpace_), 
				 0.5*layerThickTop_[ii], rmid, rout, 0.0,
				 CLHEP::twopi);
    DDLogicalPart glog2 = DDLogicalPart(solid.ddname(), matter1, solid);
#ifdef EDM_ML_DEBUG
    edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << solid.name() 
				  << " Tubs made of " << matName 
				  << " of dimensions " << rmid << ", " << rout
				  << ", " << 0.5*layerThickTop_[ii] <<", 0.0, "
				  << CLHEP::twopi/CLHEP::deg;
#endif
    zpos += 0.5*layerThickTop_[ii];
    DDTranslation r1(0,0,zpos);
    cpv.position(glog2, glog1, copy, r1, rot);
#ifdef EDM_ML_DEBUG
    edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: Position " << glog2.name()
				  << " number "	<< copy << " in "
				  << glog1.name() << " at " << r1
				  << " with " << rot;
#endif
    ++copyNumberTop_[ii];
    zpos += 0.5*layerThickTop_[ii];
  }
  if (fabs(thickTot-thick) < 0.00001) {
  } else if (thickTot > thick) {
    edm::LogError("HGCalGeom") << "Thickness of the partition "  << thick 
			       << " is smaller than " << thickTot 
			       << ": thickness of all its components in "
			       << "the top part **** ERROR ****";
  } else if (thickTot < thick) {
    edm::LogWarning("HGCalGeom") << "Thickness of the partition " << thick
				 << " does not match with " << thickTot 
				 << " of the components in top part";
  }

  // Make the bottom part next
  name  = nameM+"Bottom";
  solid = DDSolidFactory::tubs(DDName(name, nameSpace_), 
			       0.5*thick, rin, rmid, 0.0, CLHEP::twopi);
  glog1 = DDLogicalPart(solid.ddname(), matter, solid);
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << solid.name() 
				<< " Tubs made of " << matter.name() 
				<< " of dimensions " << rin << ", " << rmid 
				<< ", " << 0.5*thick << ", 0.0, " 
				<< CLHEP::twopi/CLHEP::deg;
#endif
  cpv.position(glog1, glog, 1, tran, rot);
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << glog1.name() 
				<< " number 1 positioned in " << glog.name()
				<< " at " << tran << " with " << rot;
#endif
  thickTot = 0;
  zpos     = -0.5*thick;
  for (unsigned int ly=0; ly<layerTypeBot_.size(); ++ly) {
    int     ii     = layerTypeBot_[ly];
    int     copy   = copyNumberBot_[ii];
    thickTot      += layerThickBot_[ii];
    name           = nameM+namesBot_[ii]+std::to_string(copy);    
#ifdef EDM_ML_DEBUG
    edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: Layer " << ly << ":" << ii
				  << " R " << rin << ":" << rmid << " Thick " 
				  << layerThickBot_[ly];
#endif
    DDName matName(DDSplit(materialsBot_[ii]).first, 
		   DDSplit(materialsBot_[ii]).second);
    DDMaterial matter1(matName);
    solid = DDSolidFactory::tubs(DDName(name,nameSpace_), 
				 0.5*layerThickBot_[ii], rin, rmid, 0.0,
				 CLHEP::twopi);
    DDLogicalPart glog2 = DDLogicalPart(solid.ddname(), matter1, solid);
#ifdef EDM_ML_DEBUG
    edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << solid.name() 
				  << " Tubs made of " << matName 
				  << " of dimensions " << rin << ", " << rmid 
				  << ", " << 0.5*layerThickBot_[ii] <<", 0.0, "
				  << CLHEP::twopi/CLHEP::deg;
#endif
    zpos += 0.5*layerThickBot_[ii];
    DDTranslation r1(0,0,zpos);
    cpv.position(glog2, glog1, copy, r1, rot);
#ifdef EDM_ML_DEBUG
    edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: Position " << glog2.name()
				  << " number "	<< copy << " in "
				  << glog1.name() << " at " << r1
				  << " with " << rot;
#endif
    zpos += 0.5*layerThickBot_[ii];
    ++copyNumberBot_[ii];
    if (layerSenseBot_[ly] != 0)
      positionSensitive(glog2,rin,rmid,layerSenseBot_[ly],cpv);
  }
  if (fabs(thickTot-thick) < 0.00001) {
  } else if (thickTot > thick) {
    edm::LogError("HGCalGeom") << "Thickness of the partition "  << thick 
			       << " is smaller than " << thickTot 
			       << ": thickness of all its components in "
			       << "the top part **** ERROR ****";
  } else if (thickTot < thick) {
    edm::LogWarning("HGCalGeom") << "Thickness of the partition " << thick
				 << " does not match with " << thickTot 
				 << " of the components in top part";
  }
}

void DDHGCalHEAlgo::positionSensitive(const DDLogicalPart& glog, double rin,
				      double rout, int layertype,
				      DDCompactView& cpv) {
  static const double sqrt3 = std::sqrt(3.0);
  double r    = 0.5*(waferSize_ + waferSepar_);
  double R    = 2.0*r/sqrt3;
  double dy   = 0.75*R;
  int    N    = (int)(0.5*rout/r) + 2;
  int    ium(0), ivm(0), iumAll(0), ivmAll(0), kount(0), ntot(0), nin(0);
  std::vector<int>  ntype(3,0);
  double xc[6], yc[6];
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << glog.ddname() 
				<< " rout " << rout << " N " << N 
				<< " for maximum u, v";
#endif
  for (int u = -N; u <= N; ++u) {
    int iu = std::abs(u);
    for (int v = -N; v <= N; ++v) {
      int iv = std::abs(v);
      int nr = 2*v;
      int nc =-2*u+v;
      double xpos = nc*r;
      double ypos = nr*dy;
      xc[0] = xpos+r;  yc[0] = ypos+0.5*R;
      xc[1] = xpos;    yc[1] = ypos+R;
      xc[2] = xpos-r;  yc[2] = ypos+0.5*R;
      xc[3] = xpos-r;  yc[3] = ypos-0.5*R;
      xc[4] = xpos;    yc[4] = ypos-R;
      xc[5] = xpos+r;  yc[5] = ypos-0.5*R;
      bool cornerOne(false), cornerAll(true);
      for (int k=0; k<6; ++k) {
	double rpos = std::sqrt(xc[k]*xc[k]+yc[k]*yc[k]);
	if (rpos >= rin && rpos <= rout) cornerOne = true;
	else                             cornerAll = false;
      }
      ++ntot;
      if (cornerOne) {
	int copy = iv*100 + iu;
	if (u < 0) copy += 10000;
	if (v < 0) copy += 100000;
	if (iu > ium) ium = iu;
	if (iv > ivm) ivm = iv;
	kount++;
	if (copies_.count(copy) == 0) copies_.insert(copy);
	if (cornerAll) {
	  if (iu > iumAll) iumAll = iu;
	  if (iv > ivmAll) ivmAll = iv;
	  double rpos = std::sqrt(xpos*xpos+ypos*ypos);
	  DDTranslation tran(xpos, ypos, 0.0);
	  DDRotation rotation;
	  ++nin;
	  int type = (rpos < rMaxFine_) ? 0 : ((rpos < rMinThick_) ? 1 : 2);
	  if (layertype > 1) type += 3;
	  DDName name = DDName(DDSplit(wafers_[type]).first, 
			       DDSplit(wafers_[type]).second);
	  cpv.position(name, glog.ddname(), copy, tran, rotation);
	  ++ntype[type];
#ifdef EDM_ML_DEBUG
	  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: " << name 
					<< " number " << copy
					<< " positioned in " << glog.ddname()
					<< " at " << tran 
					<< " with " << rotation;
#endif
	}
      }
    }
  }
#ifdef EDM_ML_DEBUG
  edm::LogVerbatim("HGCalGeom") << "DDHGCalHEAlgo: Maximum # of u " << ium 
				<< ":" << iumAll << " # of v " << ivm << ":" 
				<< ivmAll << " and " << nin << ":" << kount 
				<< ":" << ntot << " wafers (" << ntype[0] 
				<< ":" << ntype[1] << ":" << ntype[2] 
				<< ") for " << glog.ddname() << " R " << rin
				<< ":" << rout;
#endif
}
