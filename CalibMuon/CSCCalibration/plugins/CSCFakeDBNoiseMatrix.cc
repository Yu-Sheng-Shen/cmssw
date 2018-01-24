#include "CalibMuon/CSCCalibration/interface/CSCFakeDBNoiseMatrix.h"
#include "CondFormats/CSCObjects/interface/CSCDBNoiseMatrix.h"
#include "CondFormats/DataRecord/interface/CSCDBNoiseMatrixRcd.h"
#include "CalibMuon/CSCCalibration/interface/CSCNoiseMatrixDBConditions.h"

CSCFakeDBNoiseMatrix::CSCFakeDBNoiseMatrix(const edm::ParameterSet& iConfig)
{
  //tell the framework what data is being produced
  setWhatProduced(this,&CSCFakeDBNoiseMatrix::produceDBNoiseMatrix);  
  findingRecord<CSCDBNoiseMatrixRcd>();
}


CSCFakeDBNoiseMatrix::~CSCFakeDBNoiseMatrix()
{
}

// ------------ method called to produce the data  ------------
CSCFakeDBNoiseMatrix::Pointer
CSCFakeDBNoiseMatrix::produceDBNoiseMatrix(const CSCDBNoiseMatrixRcd& iRecord)
{
  return std::make_shared<CSCFakeDBNoiseMatrix::Pointer::element_type>( *(prefillDBNoiseMatrix()));
}

void CSCFakeDBNoiseMatrix::setIntervalFor(const edm::eventsetup::EventSetupRecordKey &, const edm::IOVSyncValue&,
						  edm::ValidityInterval & oValidity)
{
  oValidity = edm::ValidityInterval(edm::IOVSyncValue::beginOfTime(),edm::IOVSyncValue::endOfTime());
  
}
