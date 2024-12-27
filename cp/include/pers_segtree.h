#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

struct TPersistentSegtree {

    struct TPersistentSegtreeNode {
        size_t lIdx, rIdx;
        long long sum = 0;
        std::shared_ptr<TPersistentSegtreeNode> l = nullptr, r = nullptr;
        TPersistentSegtreeNode(long long lIdx, long long rIdx)
            : lIdx(lIdx), rIdx(rIdx) {
            if (lIdx != rIdx) {
                long long tmp = (lIdx + rIdx) / 2;
                l = std::make_shared<TPersistentSegtreeNode>(lIdx, tmp);
                r = std::make_shared<TPersistentSegtreeNode>(tmp, rIdx);
            }
        }

        void Copy() {
            if (l != nullptr) {
                l = std::make_shared<TPersistentSegtreeNode>(*l);
                r = std::make_shared<TPersistentSegtreeNode>(*r);
            }
        }

        void Add(size_t idx, long long value) {
            Copy();
            sum += value;
            if (l != nullptr) {
                if (idx < l->rIdx) {
                    l->Add(idx, value);
                } else {
                    r->Add(idx, value);
                }
            }
        }

        void Set(size_t idx, long long value) {
            Copy();
            if (lIdx == rIdx - 1) {
                sum = value;
                return;
            }
            if (idx < l->rIdx) {
                l->Set(idx, value);
            } else {
                r->Set(idx, value);
            }
            sum = l->sum + r->sum;
        }

        [[nodiscard]] long long Sum(size_t lQueryIdx, size_t rQueryIdx) const {
            if (lQueryIdx <= lIdx && rIdx <= rQueryIdx) {
                return sum;
            }
            if (std::max(lIdx, lQueryIdx) >= std::min(rIdx, rQueryIdx)) {
                return 0;
            }
            return l->Sum(lQueryIdx, rQueryIdx) + r->Sum(lQueryIdx, rQueryIdx);
        }
    };

    TPersistentSegtree(size_t n) : roots(n){};
    std::vector<std::shared_ptr<TPersistentSegtreeNode>> roots;
    size_t curVersion = 0;

    TPersistentSegtree() {
        roots[0] = std::make_shared<TPersistentSegtreeNode>(0, roots.size());
    }

    void Add(size_t idx, long long value, size_t version) {
        std::shared_ptr<TPersistentSegtreeNode> root =
            std::make_shared<TPersistentSegtreeNode>(*roots[version]);
        root->Add(idx, value);
        curVersion++;
        roots[curVersion] = root;
    }

    void Set(size_t idx, long long value, size_t version) {
        std::shared_ptr<TPersistentSegtreeNode> root =
            std::make_shared<TPersistentSegtreeNode>(*roots[version]);
        root->Set(idx, value);
        curVersion++;
        roots[curVersion] = root;
    }

    [[nodiscard]] long long Sum(size_t lQueryIdx, size_t rQueryIdx,
                                size_t version) const {
        return roots[version]->Sum(lQueryIdx, rQueryIdx);
    }
};