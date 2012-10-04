/*
 * Illarionserver - server for the game Illarion
 * Copyright 2011 Illarion e.V.
 *
 * This file is part of Illarionserver.
 *
 * Illarionserver  is  free  software:  you can redistribute it and/or modify it
 * under the terms of the  GNU  General  Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * Illarionserver is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY;  without  even  the  implied  warranty  of  MERCHANTABILITY  or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU  General Public License along with
 * Illarionserver. If not, see <http://www.gnu.org/licenses/>.
 */

#include "dialog/CraftingDialog.hpp"

CraftingDialog::CraftingDialog(string title, luabind::object callback): Dialog(title, "CraftingDialog", callback) {
    result = playerAborts;
    craftableIndex = 0;
    craftableAmount = 0;
}

CraftingDialog::CraftingDialog(const CraftingDialog &craftingDialog): Dialog(craftingDialog) {
    result = craftingDialog.result;
    craftableIndex = craftingDialog.craftableIndex;
    craftableAmount = craftingDialog.craftableAmount;
    groups = craftingDialog.groups;

    for (auto it = craftingDialog.getCraftablesBegin(); it != craftingDialog.getCraftablesEnd(); ++it) {
        craftables.push_back(new Craftable(**it));
    }
}

CraftingDialog::~CraftingDialog() {
    for (auto it = craftables.begin(); it != craftables.end(); ++it) {
        delete *it;
        *it = 0;
    }
}

CraftingDialog::index_t CraftingDialog::getGroupsSize() const {
    return groups.size();
}

CraftingDialog::groups_t::const_iterator CraftingDialog::getGroupsBegin() const {
    return groups.cbegin();
}

CraftingDialog::groups_t::const_iterator CraftingDialog::getGroupsEnd() const {
    return groups.cend();
}

void CraftingDialog::addGroup(std::string name) {
    if (groups.size() < 256) {
        groups.push_back(name);
    }
}

CraftingDialog::index_t CraftingDialog::getCraftablesSize() const {
    return craftables.size();
}

CraftingDialog::craftables_t::const_iterator CraftingDialog::getCraftablesBegin() const {
    return craftables.cbegin();
}

CraftingDialog::craftables_t::const_iterator CraftingDialog::getCraftablesEnd() const {
    return craftables.cend();
}

void CraftingDialog::addCraftable(uint8_t group, TYPE_OF_ITEM_ID item, string name, uint8_t secondsToCraft) {
    if (canAddCraftable(group)) {
        craftables.push_back(new Craftable(group, item, name, secondsToCraft));
    }
}

void CraftingDialog::addCraftable(uint8_t group, TYPE_OF_ITEM_ID item, string name, uint8_t secondsToCraft, uint8_t craftedStackSize) {
    if (canAddCraftable(group)) {
        craftables.push_back(new Craftable(group, item, name, secondsToCraft, craftedStackSize));
    }
}

void CraftingDialog::addCraftableIngredient(TYPE_OF_ITEM_ID item) {
    if (!craftables.empty()) {
        craftables.back()->addIngredient(item);
    }
}

void CraftingDialog::addCraftableIngredient(TYPE_OF_ITEM_ID item, uint8_t number) {
    if (!craftables.empty()) {
        craftables.back()->addIngredient(item, number);
    }
}

CraftingDialog::Result CraftingDialog::getResult() const {
    return result;
}

void CraftingDialog::setResult(Result result) {
    this->result = result;
}

CraftingDialog::index_t CraftingDialog::getCraftableIndex() const {
    return craftableIndex;
}

void CraftingDialog::setCraftableIndex(CraftingDialog::index_t index) {
    this->craftableIndex = craftableIndex;
}

Item::number_type CraftingDialog::getCraftableAmount() const {
    return craftableAmount;
}

void CraftingDialog::setCraftableAmount(Item::number_type amount) {
    this->craftableAmount = amount;
}

bool CraftingDialog::canAddCraftable(uint8_t group) {
    return (groups.size()-1 >= group) && (craftables.size() < MAXCRAFTABLES);
}

