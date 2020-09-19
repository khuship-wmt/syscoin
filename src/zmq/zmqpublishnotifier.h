// Copyright (c) 2015-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SYSCOIN_ZMQ_ZMQPUBLISHNOTIFIER_H
#define SYSCOIN_ZMQ_ZMQPUBLISHNOTIFIER_H

#include <zmq/zmqabstractnotifier.h>

class CBlockIndex;

class CZMQAbstractPublishNotifier : public CZMQAbstractNotifier
{
private:
    uint32_t nSequence {0U}; //!< upcounting per message sequence number

public:

    /* send zmq multipart message
       parts:
          * command
          * data
          * message sequence number
    */
    bool SendZmqMessage(const char *command, const void* data, size_t size);

    bool Initialize(void *pcontext) override;
    void Shutdown() override;
};

class CZMQPublishHashBlockNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyBlock(const CBlockIndex *pindex) override;
};

class CZMQPublishHashTransactionNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyTransaction(const CTransaction &transaction) override;
};

class CZMQPublishRawBlockNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyBlock(const CBlockIndex *pindex) override;
};

class CZMQPublishRawTransactionNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyTransaction(const CTransaction &transaction) override;
};
// SYSCOIN
class CZMQPublishRawMempoolTransactionNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyTransactionMempool(const CTransaction &transaction) override;
};
class CZMQPublishRawSyscoinNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifySyscoinUpdate(const char *value, const char *topic) override;
};
class CZMQPublishHashGovernanceVoteNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyGovernanceVote(const CGovernanceVote &vote) override;
};

class CZMQPublishHashGovernanceObjectNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyGovernanceObject(const CGovernanceObject &object) override;
};
class CZMQPublishRawGovernanceVoteNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyGovernanceVote(const CGovernanceVote &vote) override;
};

class CZMQPublishRawGovernanceObjectNotifier : public CZMQAbstractPublishNotifier
{
public:
    bool NotifyGovernanceObject(const CGovernanceObject &object) override;
};
#endif // SYSCOIN_ZMQ_ZMQPUBLISHNOTIFIER_H
