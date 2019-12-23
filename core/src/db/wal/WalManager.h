// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#pragma once

#include <atomic>
#include <unordered_map>
#include <src/db/meta/MetaTypes.h>
#include <thread>
//#include <src/sdk/include/MilvusApi.h>
#include "WalDefinations.h"
#include "WalFileHandler.h"
#include "WalMetaHandler.h"
#include "WalBuffer.h"

namespace milvus {
namespace engine {
namespace wal {

using TableSchemaPtr = std::shared_ptr<milvus::engine::meta::TableSchema>;
class WalManager {
 public:
    WalManager* GetInstance();
    WalManager();
    ~WalManager();


    void Init();
    //todo: return error code
    bool
    CreateTable();
    bool
    DropTable();
    bool
    Insert(const std::string &table_id,
                       size_t n,
                       const float *vectors,
                       milvus::engine::IDNumbers &vector_ids);
    void DeleteById(const std::string& table_id, const milvus::engine::IDNumbers& vector_ids);
    //not support right now
    void UpdateById(const std::string& table_id, const float* vectors, const milvus::engine::IDNumbers& vector_ids);
    void Flush();

    void Recovery();

    void BC_Thread();//unknown

    char* GetRecords();//unknown

 private:

    MXLogConfiguration mxlog_config_;
    std::atomic<uint64_t > current_lsn_;
    uint64_t last_applied_lsn_;
    std::unordered_map<std::string, TableSchemaPtr> table_meta_;
    MXLogBuffer buffer_;

    std::thread auto_flush_;
    std::thread reader_;

};
} // wal
} // engine
} // milvus

