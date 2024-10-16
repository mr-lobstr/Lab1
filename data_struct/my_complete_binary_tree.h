#ifndef MY_COMPLETE_BYNARY_TREE_H_GUARD
#define MY_COMPLETE_BYNARY_TREE_H_GUARD

#include <utility>
#include <cmath>
#include "my_queue.h"
#include "my_stack.h"

namespace data_struct
{
    template <typename T>
    class CompleteBinTree {
        struct Head {
            struct PtrTree {
                Head* pRoot = nullptr;
            };

            PtrTree left{};
            PtrTree right{};
            std::size_t cntD = 0;
        };

        using PtrTree = typename Head::PtrTree;

        struct Node: Head {
            T value{};
        };
    
    public:
        std::size_t size() const noexcept {
            return size_;
        }

        void add (T const& value) {
            if (tree.pRoot == nullptr) {
                tree.pRoot = new Node {nullptr, nullptr, 0, value};
                return;
            }

            if (is_complete (tree, depth())) {
                auto pPrev = &tree;

                while (pPrev->pRoot != nullptr) {
                    pPrev = pPrev->pRoot->
                }
            }


            Head* pHead = pRoot;
            Queue<Node*> queue;

            while (not pHead->isComplete) {
                pHead = not pHead->left->isComplete
                      ? pHead->left
                      : pHead->right;
            }

            
        }
    
    private:
        static
        auto* get_ptr_node (Head* pHead) noexcept {
            return static_cast<Node*> (pHead);
        }

        bool is_complete (PtrTree tr, std::size_t d) const noexcept {
            return pow(2, d) - 2 == tr.pHead->cntD;
        }

        std::size_t depth () const noexcept {
            return 1 + (std::size_t) log2 (size());
        }

    private:
        PtrTree tree{};
        std::size_t size_ = 0;
    };
}

#endif