﻿// Copyright (c) 2017-2018 The Syscoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SYSCOIN_SERVICES_ASSETCONSENSUS_H
#define SYSCOIN_SERVICES_ASSETCONSENSUS_H


#include <primitives/transaction.h>
#include <services/asset.h>
class CBlockIndexDB : public CDBWrapper {
public:
    CBlockIndexDB(size_t nCacheSize, bool fMemory, bool fWipe) : CDBWrapper(GetDataDir() / "blockindex", nCacheSize, fMemory, fWipe) {}
    
    bool ReadBlockHash(const uint256& txid, uint256& block_hash){
        return Read(txid, block_hash);
    } 
    bool FlushWrite(const std::vector<std::pair<uint256, uint256> > &blockIndex);
    bool FlushErase(const std::vector<uint256> &vecTXIDs);
};
class CLockedOutpointsDB : public CDBWrapper {
public:
	CLockedOutpointsDB(size_t nCacheSize, bool fMemory, bool fWipe) : CDBWrapper(GetDataDir() / "lockedoutpoints", nCacheSize, fMemory, fWipe) {}

	bool ReadOutpoint(const COutPoint& outpoint, bool& locked) {
		return Read(outpoint, locked);
	}
	bool FlushWrite(const std::vector<COutPoint> &lockedOutpoints);
	bool FlushErase(const std::vector<COutPoint> &lockedOutpoints);
};
class EthereumTxRoot {
    public:
    std::vector<unsigned char> vchBlockHash;
    std::vector<unsigned char> vchPrevHash;
    std::vector<unsigned char> vchTxRoot;
    std::vector<unsigned char> vchReceiptRoot;
    
    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {      
        READWRITE(vchBlockHash);
        READWRITE(vchPrevHash);
        READWRITE(vchTxRoot);
        READWRITE(vchReceiptRoot);
    }
};
typedef std::unordered_map<uint32_t, EthereumTxRoot> EthereumTxRootMap;
class CEthereumTxRootsDB : public CDBWrapper {
public:
    CEthereumTxRootsDB(size_t nCacheSize, bool fMemory, bool fWipe) : CDBWrapper(GetDataDir() / "ethereumtxroots", nCacheSize, fMemory, fWipe) {
       Init();
    } 
    bool ReadTxRoots(const uint32_t& nHeight, EthereumTxRoot& txRoot) {
        return Read(nHeight, txRoot);
    } 
    void AuditTxRootDB(std::vector<std::pair<uint32_t, uint32_t> > &vecMissingBlockRanges);
    bool Init();
    bool Clear();
    bool PruneTxRoots(const uint32_t &fNewGethSyncHeight);
    bool FlushErase(const std::vector<uint32_t> &vecHeightKeys);
    bool FlushWrite(const EthereumTxRootMap &mapTxRoots);
};
typedef std::vector<std::pair<uint64_t, uint32_t> > EthereumMintTxVec;
class CEthereumMintedTxDB : public CDBWrapper {
public:
    CEthereumMintedTxDB(size_t nCacheSize, bool fMemory, bool fWipe) : CDBWrapper(GetDataDir() / "ethereumminttx", nCacheSize, fMemory, fWipe) {
    } 
    bool ExistsKey(const std::pair<uint64_t, uint32_t> &ethKey) {
        return Exists(ethKey);
    } 
    bool FlushErase(const EthereumMintTxVec &vecMintKeys);
    bool FlushWrite(const EthereumMintTxVec &vecMintKeys);
};
extern std::unique_ptr<CBlockIndexDB> pblockindexdb;
extern std::unique_ptr<CLockedOutpointsDB> plockedoutpointsdb;
extern std::unique_ptr<CEthereumTxRootsDB> pethereumtxrootsdb;
extern std::unique_ptr<CEthereumMintedTxDB> pethereumtxmintdb;
bool DisconnectSyscoinTransaction(const CTransaction& tx, const CBlockIndex* pindex, CCoinsViewCache& view, AssetMap &mapAssets, AssetSupplyStatsMap &mapAssetSupplyStats, AssetAllocationMap &mapAssetAllocations, EthereumMintTxVec &vecMintKeys);
bool DisconnectAssetActivate(const CTransaction &tx, AssetMap &mapAssets, AssetSupplyStatsMap &mapAssetSupplyStats);
bool DisconnectAssetSend(const CTransaction &tx, AssetMap &mapAssets, AssetAllocationMap &mapAssetAllocations);
bool DisconnectAssetUpdate(const CTransaction &tx, AssetMap &mapAssets);
bool DisconnectAssetTransfer(const CTransaction &tx, AssetMap &mapAssets);
bool DisconnectAssetAllocation(const CTransaction &tx, AssetSupplyStatsMap &mapAssetSupplyStats, AssetAllocationMap &mapAssetAllocations);
bool DisconnectMintAsset(const CTransaction &tx, AssetSupplyStatsMap &mapAssetSupplyStats, AssetAllocationMap &mapAssetAllocations, EthereumMintTxVec &vecMintKeys);
bool CheckSyscoinMint(const bool ibd, const CTransaction& tx, std::string& errorMessage, const bool &fJustCheck, const bool& bSanity, const bool& bMiner, const int& nHeight, const uint256& blockhash, AssetMap& mapAssets, AssetSupplyStatsMap &mapAssetSupplyStats, AssetAllocationMap &mapAssetAllocations, EthereumMintTxVec &vecMintKeys, bool &bTxRootError);
bool CheckAssetInputs(const CTransaction &tx, const CCoinsViewCache &inputs, bool fJustCheck, int nHeight, const uint256& blockhash, AssetMap &mapAssets, AssetAllocationMap &mapAssetAllocations, AssetTXPrevOutPointMap &mapAssetTXPrevOutPoints, std::string &errorMessage, const bool &bSanityCheck=false, const bool &bMiner=false);
bool CheckSyscoinInputs(const CTransaction& tx, CValidationState& state, const CCoinsViewCache &inputs, const bool &fJustCheck, int nHeight, const bool &bSanity, bool& bSenderConflict);
bool CheckSyscoinInputs(const bool ibd, const CTransaction& tx, CValidationState &state, const CCoinsViewCache &inputs, const bool &fJustCheck, bool &bSenderConflict, int nHeight, const uint32_t &nTime, const uint256 & blockHash, const bool &bSanity, const bool &bMiner, AssetAllocationMap &mapAssetAllocations, AssetTXPrevOutPointMap &mapAssetTXPrevOutPoints, AssetMap &mapAssets, AssetSupplyStatsMap &mapAssetSupplyStats, EthereumMintTxVec &vecMintKeys, std::vector<COutPoint> &vecLockedOutpoints);
static CAssetAllocation emptyAllocation;
bool ResetAssetAllocation(const std::string &senderStr, const uint256 &txHash, const bool &bMiner=false, const bool &bExpiryOnly=false);
void ResyncAssetAllocationStates();
bool CheckSyscoinLockedOutpoints(const CTransactionRef &tx, CValidationState& state);
bool CheckAssetAllocationInputs(const CTransaction &tx, const CCoinsViewCache &inputs, bool fJustCheck, int nHeight, const uint256& blockhash, AssetSupplyStatsMap &mapAssetSupplyStats, AssetAllocationMap &mapAssetAllocations, AssetTXPrevOutPointMap &mapAssetTXPrevOutPoints, std::vector<COutPoint> &vecLockedOutpoints, std::string &errorMessage, bool& bSenderConflict, const bool &bSanityCheck = false, const bool &bMiner = false);
#endif // SYSCOIN_SERVICES_ASSETCONSENSUS_H
