/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "Storage/IStorage.hpp"
#include "CommonTypes.hpp"
#include "PropertyTree.hpp"
#include "TableId.hpp"
#include "TableBase.hpp"

namespace D2ModGen {

struct TableSet {
    std::map<TableId, Table> tables;
    StringSet                relativeNames;
};
class TableView;
struct DataContext {
    TableSet                                       tableSet;  // txt tables parsed into csv-like tables.
    std::map<std::string, PropertyTree>            jsonFiles; // data of extra json files
    std::map<std::string, IStorage::StoredFileRef> copyFiles; // files for plain copy.
    std::map<TableId, std::shared_ptr<TableView>>  pluginTables;

    DataContext();
    ~DataContext();

    bool readData(const IStorage::StoredData& data);
    bool writeData(IStorage::StoredData& data) const;

    bool mergeWith(const DataContext& source, ConflictPolicy policy);
};

}
