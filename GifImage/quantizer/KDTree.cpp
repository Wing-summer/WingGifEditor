//
// Created by succlz123 on 2017/12/28.
//

#include <algorithm>
#include "KDTree.h"

using namespace blk;

// euclideanDistance
// int distance = nr * nr + ng * ng + nb * nb;
// manhattanDistance
// int distance = abs(nr) + abs(ng) + abs(nb);
int calculateDist(KDTree::Node *node, uint8_t r, uint8_t g, uint8_t b) {
    int tmp = 0;
    int diff = node->r - r;
    tmp += (diff * diff) << 1;
    diff = node->g - g;
    tmp += (diff * diff) << 2;
    diff = node->b - b;
    tmp += (diff * diff) * 3;
    return tmp;
}

uint8_t getDimension(const std::vector<ARGB> &rgb, int start, int end) {
    int dataSize = end - start + 1;
    if (dataSize <= 0) {
        return 0;
    }
    int sumR = 0;
    int sumG = 0;
    int sumB = 0;

    for (int i = 0; i < dataSize; ++i) {
        sumR += rgb[i].r;
        sumG += rgb[i].g;
        sumB += rgb[i].b;
    }

    int rAve = sumR / dataSize;
    int gAve = sumG / dataSize;
    int bAve = sumB / dataSize;

    for (int i = 0; i < dataSize; ++i) {
        auto color = rgb[i];
        int qr = color.r;
        int qg = color.g;
        int qb = color.b;
        sumR += (qr - rAve) * (qr - rAve);
        sumG += (qg - gAve) * (qg - gAve);
        sumB += (qb - bAve) * (qb - bAve);
    }

    rAve = sumR / dataSize;
    gAve = sumG / dataSize;
    bAve = sumB / dataSize;

    int maxVariance = rAve;
    uint8_t dimension = 0;
    if (gAve > maxVariance) {
        maxVariance = gAve;
        dimension = 1;
    }
    if (bAve > maxVariance) {
        dimension = 2;
    }
    return dimension;
}

void *KDTree::createKDTree(Node *node, std::vector<ARGB> &rgb, int32_t start, int32_t end, uint8_t split) {
    int size = end - start + 1;
    if (size <= 0) {
        return nullptr;
    }

    if (size == 1) {
        node->r = rgb[start].r;
        node->g = rgb[start].g;
        node->b = rgb[start].b;
        node->index = rgb[start].index;
        node->split = split;
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }

    std::sort(rgb.begin() + start, rgb.begin() + end, Compare(split));

    int splitSize = size / 2;
    int leftStart = start;
    int leftEnd = start + splitSize - 1;
    int rightStart = leftEnd + 2;
    int rightEnd = end;
    int current = start + splitSize;

    node->r = rgb[current].r;
    node->g = rgb[current].g;
    node->b = rgb[current].b;
    node->index = rgb[current].index;
    node->split = split;

    // (split + 1) % 3
    uint8_t leftSplit = getDimension(rgb, leftStart, leftEnd);
    uint8_t rightSplit = getDimension(rgb, rightStart, rightEnd);

    auto *leftNode = new Node();
    auto *rightNode = new Node();
    node->left = leftNode;
    node->right = rightNode;
    createKDTree(leftNode, rgb, leftStart, leftEnd, leftSplit);
    createKDTree(rightNode, rgb, rightStart, rightEnd, rightSplit);
    return node;
}

int KDTree::searchNoBacktracking(KDTree::Node *node, uint8_t r, uint8_t g, uint8_t b, int32_t dis) {
    if (node == nullptr) {
        return dis;
    }
    if (node->left == nullptr && node->right == nullptr) {
        if (dis < 0) {
            nearest = *node;
            dis = calculateDist(node, r, g, b);
            return dis;
        }
    }
    bool comp = false;
    switch (node->split) {
        case 0:
            comp = node->r <= r;
            break;
        case 1:
            comp = node->g <= g;
            break;
        case 2:
            comp = node->b <= b;
            break;
        default:
            break;
    }
    if (comp) {
        dis = searchNoBacktracking(node->left, r, g, b, dis);
        int tmp = calculateDist(node, r, g, b);
        if (tmp < dis || dis == -1) {
            nearest = *node;
            dis = tmp;
        }
    } else {
        dis = searchNoBacktracking(node->right, r, g, b, dis);
        int tmp = calculateDist(node, r, g, b);
        if (tmp < dis || dis == -1) {
            nearest = *node;
            dis = tmp;
        }
    }
    return dis;
}

void KDTree::freeKDTree(KDTree::Node *tree) {
    if (tree == nullptr) {
        return;
    }
    freeKDTree(tree->left);
    freeKDTree(tree->right);
    delete tree->left;
    delete tree->right;
}
