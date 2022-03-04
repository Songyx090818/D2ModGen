/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "Storage/IStorage.hpp"
#include "CommonTypes.hpp"
#include "PropertyTree.hpp"

#include <deque>

namespace D2ModGen {

struct TableCell {
    std::string str;

    TableCell()                 = default;
    TableCell(const TableCell&) = default;
    TableCell(std::string s)
        : str(std::move(s))
    {}

    int  toInt() const;
    void setInt(int value);
    bool isEmpty() const { return str.empty(); }
    void clear() { str.clear(); }

    std::string toLower() const;

    bool startsWith(const std::string& s) const;
    bool endsWith(const std::string& s) const;
    bool contains(const std::string& s) const;

    bool operator==(const std::string& s) const { return s == str; }
    bool operator!=(const std::string& s) const { return s != str; }
};

struct TableRow {
    std::vector<TableCell> data;
    TableRow() = default;
    explicit TableRow(int size)
    {
        data.resize(size);
    }
    explicit TableRow(std::vector<TableCell> data)
        : data(std::move(data))
    {
    }
};

struct Table {
    std::string              id;
    std::deque<TableRow>     rows;
    std::vector<std::string> columns;
    bool                     modified    = false;
    bool                     forceOutput = false;

    int indexOf(const std::string& col) const;
};

struct TableSet {
    std::map<std::string, Table> tables;
    StringSet                    relativeNames;
};

struct DataContext {
    TableSet                                       tableSet;  // txt tables parsed into csv-like tables.
    std::map<std::string, PropertyTree>            jsonFiles; // data of extra json files
    std::map<std::string, IStorage::StoredFileRef> copyFiles; // files for plain copy.

    bool readData(const IStorage::StoredData& data);
    bool writeData(IStorage::StoredData& data) const;

    bool mergeWith(const DataContext& source, ConflictPolicy policy);
};

}
