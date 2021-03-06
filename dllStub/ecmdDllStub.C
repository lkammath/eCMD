//IBM_PROLOG_BEGIN_TAG
/* 
 * Copyright 2003,2016 IBM International Business Machines Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * 	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//IBM_PROLOG_END_TAG



//--------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------
#include <inttypes.h>
#include <stdio.h>

#include <ecmdDllCapi.H>
#include <ecmdStructs.H>
#include <ecmdReturnCodes.H>
#include <ecmdDataBuffer.H>
#ifdef ECMD_FAPI2
#include <fapi2DllCapi.H>
#endif

//--------------------------------------------------------------------
//  Forward References                                                
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//  Function Definitions                                               
//--------------------------------------------------------------------
/* For use by dllQueryConfig and dllQueryExist */
uint32_t queryConfigExist(ecmdChipTarget & target, ecmdQueryData & queryData, ecmdQueryDetail_t detail, bool allowDisabled);

//--------------------------------------------------------------------
//  Function Definitions                                               
//--------------------------------------------------------------------
//
//   These are just stubs, used for testing the out the DLL

uint32_t dllInitDll() {
  /* This is where we would init any local variables to the dll */
  return ECMD_SUCCESS;
}

uint32_t dllFreeDll() {
  return ECMD_SUCCESS;
}

uint32_t dllSpecificCommandArgs(int* argc, char** argv[]) {
  return ECMD_SUCCESS;
}

/* Dll Specific Return Codes */
std::string dllSpecificParseReturnCode(uint32_t i_returnCode) {
  return ""; 
}

uint32_t dllGetRing (ecmdChipTarget & target, const char * ringName, ecmdDataBuffer & data) {
  return ECMD_SUCCESS;
}

uint32_t dllGetRingSparse(ecmdChipTarget & i_target, const char * i_ringName, ecmdDataBuffer & o_data, ecmdDataBuffer & i_mask, uint32_t i_flags) 
{ 
    return ECMD_SUCCESS; 
} 

uint32_t dllPutRingSparse(ecmdChipTarget & i_target, const char * i_ringName, ecmdDataBuffer & i_data, ecmdDataBuffer & i_mask, uint32_t i_flags) 
{ 
    return ECMD_SUCCESS; 
} 

uint32_t dllGetRingHidden(ecmdChipTarget & i_target, const char * i_ringName, ecmdDataBuffer & o_data, uint32_t i_mode)
{ 
    return ECMD_SUCCESS; 
} 

uint32_t dllPutRingHidden(ecmdChipTarget & i_target, const char * i_ringName, ecmdDataBuffer & i_data, uint32_t i_mode) 
{ 
    return ECMD_SUCCESS; 
} 

uint32_t dllPutRing (ecmdChipTarget & target, const char * ringName, ecmdDataBuffer & data) {
  return ECMD_SUCCESS;
}

uint32_t dllGetScom (ecmdChipTarget & target, uint64_t address, ecmdDataBuffer & data) {
  return ECMD_SUCCESS;
}

uint32_t dllPutScom (ecmdChipTarget & target, uint64_t address, ecmdDataBuffer & data) {
  return ECMD_SUCCESS;
}

uint32_t dllGetArray (ecmdChipTarget & target, const char * arrayName, uint32_t * address, ecmdDataBuffer & data) {
  return ECMD_SUCCESS;
}

uint32_t dllPutArray (ecmdChipTarget & target, const char * arrayName, uint32_t * address, ecmdDataBuffer & data) {
  return ECMD_SUCCESS;
}

/* ##################################################################### */
/* Query Functions - Query Functions - Query Functions - Query Functions */
/* ##################################################################### */
uint32_t dllQueryConfig(ecmdChipTarget & i_target, ecmdQueryData & o_queryData, ecmdQueryDetail_t i_detail) {
  return queryConfigExist(i_target, o_queryData, i_detail, false);
}

uint32_t dllQueryExist(ecmdChipTarget & i_target, ecmdQueryData & o_queryData, ecmdQueryDetail_t i_detail) {
  return queryConfigExist(i_target, o_queryData, i_detail, true);
}

uint32_t queryConfigExist(ecmdChipTarget & target, ecmdQueryData & queryData, ecmdQueryDetail_t detail, bool allowDisabled) {

  // queryConfig is a central function of any eCMD plugin.  The data from this function drives
  // the eCMD chip loopers in the cmdline or any other eCMD program.
  // This creates a complex set of nested structures that represents all the available targets in a system
  // It can be filted down by the state variables of the target passed in

  // This version for the stub just creates a small set of fake data

  // The stack of target data variables
  ecmdThreadData threadData;
  ecmdChipUnitData chipUnitData;
  ecmdChipData chipData;
  ecmdNodeData nodeData;
  ecmdSlotData slotData;
  ecmdCageData cageData;

  /* Let's return some dummy info, we will return a proc with cores and threads */

  // Setup the threadData
  threadData.threadId = 0;

  // Setup the chipUnitData
  chipUnitData.chipUnitType = "core";
  chipUnitData.chipUnitNum = 0;
  chipUnitData.numThreads = 2;
  chipUnitData.threadData.push_front(threadData);

  // Setup the chipData
  chipData.chipUnitData.push_front(chipUnitData);
  chipData.chipType = "pu";
  chipData.pos = 0;
  slotData.chipData.push_front(chipData);

  // Create positions 1-3 without any chipunit data
  ecmdChipData cdReplicate;
  cdReplicate.chipUnitData.clear();
  cdReplicate.chipType = "pu";
  cdReplicate.pos = 1;
  slotData.chipData.push_back(cdReplicate);

  cdReplicate.pos = 2;
  slotData.chipData.push_back(cdReplicate);

  cdReplicate.pos = 3;
  slotData.chipData.push_back(cdReplicate);

  // Define slot 0
  slotData.slotId = 0;

  // Setup the node
  nodeData.nodeId = 0;
  nodeData.slotData.push_front(slotData);

  // Setup the cage
  cageData.cageId = 0;
  cageData.nodeData.push_front(nodeData);

  // Add it all to the return structs
  queryData.cageData.push_front(cageData);
  
  return ECMD_SUCCESS;
} 

uint32_t dllQueryRing(ecmdChipTarget & i_target, std::list<ecmdRingData> & o_queryData, const char * i_ringName, ecmdQueryDetail_t i_detail) {

  ecmdRingData ringData;

  ringData.ringNames.push_back("ring1");
  ringData.address = 0x80000001;
  ringData.bitLength = 100;
  ringData.isCheckable = false;
  ringData.clockState = ECMD_CLOCKSTATE_ON;

  o_queryData.push_back(ringData);

  ringData.ringNames.clear();
  ringData.ringNames.push_back("ring2");
  ringData.ringNames.push_back("ring2long");
  ringData.address = 0x80000002;
  ringData.bitLength = 2000;
  ringData.isCheckable = true;
  ringData.clockState = ECMD_CLOCKSTATE_NA;

  o_queryData.push_back(ringData);

  return ECMD_SUCCESS;
}

uint32_t dllQueryArray(ecmdChipTarget & target, ecmdArrayData & queryData, const char * arrayName) {
  return ECMD_SUCCESS;
} 

uint32_t dllQueryFileLocation(ecmdChipTarget & i_target, ecmdFileType_t i_fileType, std::string & o_fileLocation, std::string & io_version) {
  return ECMD_SUCCESS;
} 

uint32_t dllQueryFileLocationHidden(ecmdChipTarget & i_target, ecmdFileType_t i_fileType, std::list<std::pair<std::string, std::string> > & o_fileLocations, std::string & io_version) {
  return ECMD_SUCCESS;
} 

uint32_t dllQueryFileLocationHidden2(const ecmdChipTarget & i_target, ecmdFileType_t i_fileType, std::list<ecmdFileLocation> & o_fileLocations, std::string & io_version) {
  o_fileLocations.push_back((ecmdFileLocation){ "bogus/file.text", "bogus/file.hash" });
  io_version = "v8";
  return ECMD_SUCCESS;
}

uint32_t dllFlushSys () {
  return ECMD_SUCCESS;
} 

uint32_t dllIplSys () {
  return ECMD_SUCCESS;
}

void dllOutputError(const char* message) {
  printf("DLLSTUBERROR : %s\n",message);
}

void dllOutputWarning(const char* message) {
  printf("DLLSTUBWARNING : %s\n",message);
}

void dllOutput(const char* message) {
  printf("%s", message);
}

uint32_t dllGetChipData(ecmdChipTarget & i_target, ecmdChipData & o_data) {
  return ECMD_SUCCESS;
}

uint32_t dllEnableRingCache(ecmdChipTarget & i_target) {
  return ECMD_SUCCESS;
}

uint32_t dllDisableRingCache(ecmdChipTarget & i_target) {
  return ECMD_SUCCESS;
}

uint32_t dllFlushRingCache(ecmdChipTarget & i_target) {
  return ECMD_SUCCESS;
}

bool dllIsRingCacheEnabled(ecmdChipTarget & i_target) {
  return false;
}

uint32_t dllGetArray(ecmdChipTarget & i_target, const char * i_arrayName, ecmdDataBuffer & i_address, ecmdDataBuffer & o_data) {
  return ECMD_SUCCESS;
}

uint32_t dllPutArray(ecmdChipTarget & i_target, const char * i_arrayName, ecmdDataBuffer & i_address, ecmdDataBuffer & i_data) {
  return ECMD_SUCCESS;
}

/* ################################################################# */
/* Misc Functions - Misc Functions - Misc Functions - Misc Functions */
/* ################################################################# */
uint32_t dllGetScandefOrder(ecmdChipTarget & i_target, uint32_t & o_mode) {
  uint32_t rc = ECMD_SUCCESS;
  ecmdChipData chipData;

  /* Just get the chip data, we get extra data but this has never been a performance problem for us */
  rc = dllGetChipData(i_target, chipData);
  if (rc) return rc;

  o_mode = chipData.chipFlags & ECMD_CHIPFLAG_BUSMASK;

  return rc;
}

void dllSetTraceMode(ecmdTraceType_t i_type, bool i_enable) {
  dllOutputError("Not supported: dllSetTraceMode");
}

bool dllQueryTraceMode(ecmdTraceType_t i_type) {
  return false;
}

uint32_t dllTargetTranslateNormalToFused(ecmdChipTarget & i_target, uint32_t & o_core, uint32_t & o_thread) {
  uint32_t rc = ECMD_SUCCESS;
  return rc;
}

/* ######################################################################### */
/* UnitID Functions - UnitID Functions - UnitID Functions - UnitID Functions */
/* ######################################################################### */
/* Added to fix symbol errors - JTA 06/28/14 */
uint32_t dllTargetToUnitId(ecmdChipTarget & io_target) {
  uint32_t rc = ECMD_SUCCESS;
  return rc;
}

uint32_t dllUnitIdStringToTarget(std::string i_unitId, std::list<ecmdChipTarget> & o_target) {
  uint32_t rc = ECMD_SUCCESS;
  return rc;
}

uint32_t dllUnitIdToTarget(uint32_t i_unitId, std::list<ecmdChipTarget> & o_target) {
  uint32_t rc = ECMD_SUCCESS;
  return rc;
}

uint32_t dllUnitIdToString(uint32_t i_unitId, std::string & o_unitIdStr) {
  uint32_t rc = ECMD_SUCCESS;
  return rc;
}

uint32_t dllSequenceIdToTarget(uint32_t i_core_seq_num, ecmdChipTarget & io_target, uint32_t i_thread_seq_num) {
  uint32_t rc = ECMD_SUCCESS;
  return rc;
}

uint32_t dllTargetToSequenceId(ecmdChipTarget i_target, uint32_t & o_core_seq_num, uint32_t & o_thread_seq_num) {
  uint32_t rc = ECMD_SUCCESS;
  return rc;
}

uint32_t dllGetUnitIdVersion(uint32_t & o_unitIdVersion) {
  uint32_t rc = ECMD_SUCCESS;
  o_unitIdVersion = 0x10002030;
  return rc;
}

uint32_t dllQueryClockState(ecmdChipTarget &i_target, const char *i_clockDomain, ecmdClockState_t &o_clockState) {
  uint32_t rc = ECMD_SUCCESS;
  return rc;
}

uint32_t dllRelatedTargets(const ecmdChipTarget & i_target, const std::string i_relatedType, std::list<ecmdChipTarget> & o_relatedTargets, const ecmdLoopMode_t i_mode = ECMD_DYNAMIC_LOOP) {
  o_relatedTargets.push_back(i_target);
  return ECMD_SUCCESS;
}

#ifdef ECMD_FAPI2
uint32_t dllFapi2AttributeStringToId(std::string i_attrString, fapi2::AttributeId &o_attrId) {
  if (i_attrString != "ATTR_CHIP_ID")
    return 0x0206005A; // FAPI_UNSUPPORTED_ATTRIBUTE
  o_attrId = fapi2::ATTR_CHIP_ID;
  return ECMD_SUCCESS;
}

uint32_t dllFapi2GetAttribute (const ecmdChipTarget &i_target, const uint32_t i_id, fapi2::AttributeData &o_data)
{
  o_data.faUint32 = 42;
  return ECMD_SUCCESS;
}

uint32_t dllFapi2SetAttribute (const ecmdChipTarget &i_target, const uint32_t i_id, fapi2::AttributeData &i_data)
{
  return ECMD_SUCCESS;
}

uint32_t dllFapi2GetAttrInfo (fapi2::AttributeId i_attrId, uint32_t &o_attrType, uint32_t &o_numOfEntries, uint32_t &o_numOfBytes, bool &o_attrEnum)
{
  o_attrType = FAPI_ATTRIBUTE_TYPE_UINT32;
  o_numOfEntries = 1;
  o_numOfBytes = 4;
  o_attrEnum = false;
  return ECMD_SUCCESS;
}
#endif // ECMD_FAPI2
