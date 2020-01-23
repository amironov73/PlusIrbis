// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "irbis.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace irbis {

static std::size_t arrangeLevel(std::vector<TreeNode> &list, int level, std::size_t index, std::size_t count) {
    std::size_t next = index + 1;
    int level2 = level + 1;

    TreeNode &parent = list[index];
    while (next < count) {
        TreeNode &child = list[next];
        if (child.level <= level) {
            break;
        }

        if (child.level == level2) {
                parent.children.push_back(child);
        }

        next++;
    }

    return next;
}

static void arrangeLevel(std::vector<TreeNode> &list, int level) {
    std::size_t count = list.size();
    std::size_t index = 0;

    while (index < count) {
        std::size_t next = arrangeLevel(list, level, index, count);
        index = next;
    }
}

static int countIndent(const String &text) {
    int result = 0;
    for (wchar_t c : text) {
        if (c == L'\u0009') {
            result++;
        } else {
            break;
        }
    }

    return result;
}

void TreeFile::parse(const StringList &lines) {
    roots.clear();
    std::vector<TreeNode> list;
    if (lines.empty()) {
        return;
    }

    auto &firstLine = lines[0];
    if (countIndent(firstLine)) {
        throw IrbisException();
    }
    TreeNode node1;
    node1.value = firstLine;
    list.push_back(node1);

    auto currentLevel = 0;
    for (std::size_t i = 1; i < lines.size(); i++) {
        auto line = lines[i];
        if (line.empty()) {
            return;
        }

        const auto level = countIndent(line);
        if (level > (currentLevel + 1)) {
            throw IrbisException();
        }

        currentLevel = level;
        // line = line.trimmed(); // TODO: trimStart()
        TreeNode node2;
        node2.value = line;
        node2.level = level;
        list.push_back(node2);
    }

    auto maxLevel = list[0].level;
    for (const auto &node : list) {
        if (node.level > maxLevel) {
            maxLevel = node.level;
        }
    }

    for (int level = 0; level < maxLevel; ++level) {
        arrangeLevel(list, level);
    }

    for (auto &node : list) {
        if (node.level == 0) {
            roots.push_back(node);
        }
    }
}

}