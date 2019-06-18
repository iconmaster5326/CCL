/*
 * gc.cpp
 *
 *  Created on: Jun 14, 2019
 *      Author: iconmaster
 */

#include <unordered_set>
#include <unordered_map>
#include <limits>

#include "ccl/gc.hpp"

using namespace std;
using namespace ccl;

std::list<ccl::GcNode> ccl::gcNodes{};
std::unordered_set<GcNodeRef,GcNodeRefHash>* ccl::gcValidNodes = nullptr;
std::list<GcNode>* ccl::gcStaticallyAllocatedNodes = nullptr;
Mutex ccl::gcMutex{};
size_t ccl::gcBytesAllocated = 0;
size_t ccl::gcHardLimit = numeric_limits<size_t>::max();
size_t ccl::gcSoftLimit = 0;

ccl::GcAble::~GcAble() {}

void ccl::GcAble::gcGetChildren(std::function<void(GcNodeRef)> addChild) {}

void ccl::GcNode::runMinorGC() {
	Lock lock{gcMutex};
	
	begin:
	size_t memAtBegin = gcBytesAllocated;
	
	for (GcNodeRef it = gcNodes.begin(); it != gcNodes.end(); it++) {
		if (it->collectable) {
			GcNodeRef node = it; it++;
			
			if (!gcValidNodes) gcValidNodes = new std::unordered_set<GcNodeRef,GcNodeRefHash>();
			gcValidNodes->erase(node);
			gcBytesAllocated -= node->size;
			delete node->memory;
			gcNodes.erase(node);
			
			it--;
		}
	}
	
	if (memAtBegin != gcBytesAllocated) goto begin;
}

static int nodeID(GcNodeRef node) {
	int i = 0;
	for (GcNodeRef node2 = gcNodes.begin(); node2 != gcNodes.end(); node2++) {
		if (node == node2) return i;
		i++;
	}
	return -1;
}

void ccl::GcNode::runMajorGC() {
	Lock lock{gcMutex};
	
	CCL_GC_PRINT("\ttotal nodes: " << gcNodes.size());
	unordered_set<GcNodeRef,GcNodeRefHash> roots;
	
	// find all root nodes by counting references
	// (a root is a node with more refs than it has parents)
	{
		unordered_map<GcNodeRef,size_t,GcNodeRefHash> refCounts;
		
		for (GcNodeRef node = gcNodes.begin(); node != gcNodes.end(); node++) {
			node->memory->gcGetChildren([&](GcNodeRef child) {
				if (!GcNode::valid(child)) return;
				refCounts[child]++;
			});
		}
		
		for (GcNodeRef node = gcNodes.begin(); node != gcNodes.end(); node++) {
			if (node->refs > refCounts[node]) {
				CCL_GC_PRINT("\tnode " << nodeID(node) << " is a root: " << node->refs << " > " << refCounts[node]);
				roots.insert(node);
			}
		}
	}
	
	// find the closure of the set of roots
	bool expanded = true;
	while (expanded) {
		expanded = false;
		
		for (auto it = roots.begin(); it != roots.end(); it++) {
			GcNodeRef node = *it;
			node->memory->gcGetChildren([&](GcNodeRef child) {
				if (!GcNode::valid(child)) return;
				if (roots.find(child) == roots.end()) {
					CCL_GC_PRINT("\tnode " << nodeID(child) << " is reachable; node " << nodeID(node) << " points to it");
					roots.insert(child);
					expanded = true;
				}
			});
			if (expanded) break;
		}
	}
	
	// anything not in this set is collectible
	for (GcNodeRef node = gcNodes.begin(); node != gcNodes.end(); node++) {
		if (roots.find(node) == roots.end()) {
			CCL_GC_PRINT("\tnode " << nodeID(node) << " in a cycle");
			node->collectable = true;
		}
	}
	
	// run minor GC; all unreferenced cycles should now be collected
	runMinorGC();
}

bool ccl::GcNode::valid(GcNodeRef node) {
	Lock lock{gcMutex};
	if (!gcValidNodes) {
		gcValidNodes = new std::unordered_set<GcNodeRef,GcNodeRefHash>();
		return false;
	}
	return gcValidNodes->find(node) != gcValidNodes->end();
}
