#ifndef COMPLETE_BINARY_TREE_H_GUARD
#define COMPLETE_BINARY_TREE_H_GUARD

#include <cstdint>
#include <cmath>
#include "fbinary_tree.h"
#include "queue.h"

namespace binary_detail
{
    template <typename T, typename Tree>
    struct IterImpl {
        using Container = Tree;
        using CIter = typename Tree::ConstNodeIt;

    public:
        IterImpl() noexcept = default;

        IterImpl (CIter root) {
            queue.emplace_back (root);
        }

        CIter real() const noexcept {
            return queue.front();
        }

        bool equal (IterImpl const& rhs) const noexcept {
            if (is_end() or rhs.is_end()){
                return is_end() == rhs.is_end();
            }

            return real() == rhs.real();
        }

        void next() {
            if (real().left() != CIter{}) {
                queue.push_back (real().left());
                
                if (real().right() != CIter{}) {
                    queue.push_back (real().right());
                }
            }
            queue.pop_front();
        }

        T const& get_value() const noexcept {
            return real()->value;
        } 
    
    private:
        bool is_end() const noexcept {
            return queue.empty();
        }

    private:
        data_struct::Queue<CIter> queue{}; 
    };
}

namespace data_struct
{
    template <typename T>
    class CompleteBinTree {
        struct Node {
            std::size_t childCnt;
            T value;
        };

        using Impl = FBinTree<Node, CompleteBinTree>;

        using NodeIt = typename Impl::Iterator;
        using ConstNodeIt = typename Impl::ConstIterator;

        using IterImpl = binary_detail::IterImpl<T, CompleteBinTree>;
        friend IterImpl;
    
    public:
        using ConstIterator = ForwardIterator<T, IterImpl, Const_tag>;

    public:
        CompleteBinTree() noexcept = default;

        CompleteBinTree (CompleteBinTree&& rhs) noexcept
            : impl (std::move (rhs.impl))
            , height (std::exchange (rhs.height, 0))
        {}

        CompleteBinTree (CompleteBinTree const& rhs)
            : impl ()
        {
            algs::copy (rhs.begin(), rhs.end(), algs::add_inserter (*this));
        }

        template <class Iter, class = EnableIfForward<Iter>>
        CompleteBinTree (Iter beg, Iter end) {
            algs::copy (beg, end, algs::add_inserter (*this));
        }

        CompleteBinTree (std::initializer_list<T> iList) {
            algs::copy (iList.begin(), iList.end(), algs::add_inserter (*this));
        }

        CompleteBinTree (std::size_t count, T const& value = T()) {
            while (count--) {
                add (value);
            }
        }

        CompleteBinTree& operator= (CompleteBinTree&& rhs) noexcept {
            if (this != &rhs) {
                auto tmp = std::move (rhs);
                swap (tmp);
            } 
            return *this;
        }

        CompleteBinTree& operator= (CompleteBinTree const& rhs) {
            if (this != &rhs) {
                auto tmp = rhs;
                swap (tmp);
            }
            return *this;
        }

        ~CompleteBinTree() noexcept {
            while (not empty()) {
                pop_back();
            }
        }
        bool empty() const noexcept {
            return impl.empty();
        }

        std::size_t size() const noexcept {
            return not empty() ? impl.root()->childCnt + 1 : 0;
        }

        void swap (CompleteBinTree& rhs) noexcept {
            std::swap (impl, rhs.impl);
            std::swap (height, rhs.height);
        }

        auto begin() const noexcept {
            return empty()
                 ? ConstIterator {}
                 : ConstIterator (impl.root());
        }

        auto end() const noexcept {
            return ConstIterator {};
        }
   
        template <typename... Ts>
        void emplace_add (Ts&&... params) {
            auto it = insertPosition();
            impl.emplace_node (
                it, 0ull, std::forward<Ts> (params)...
            );
        }

        void add (T&& value) {
            emplace_add (std::move (value));
        }

        void add (T const& value) {
            emplace_add (value);
        }

        void pop_back() noexcept {

            auto it = deletePosition();
            impl.erase_node (it);
        }
    private:
        ConstNodeIt insertPosition() noexcept {
            if (empty())
                return insertIntoEmpty();

            if (is_complete (impl.root(), height))
                return insertIntoComplete();

            return insertIntoIncomplete();
        }
        
        bool is_complete (ConstNodeIt it, std::size_t height) const noexcept {
            return std::pow (2, height) - 2 == it->childCnt; 
        }

        ConstNodeIt insertIntoEmpty () noexcept {
            height = 1;
            return impl.root();
        }

        ConstNodeIt insertIntoComplete () noexcept {
            auto it = impl.root();

            while (it != impl.end()) {
                ++it->childCnt;
                it.to_left();
            }

            ++height;
            return it;
        }

        ConstNodeIt insertIntoIncomplete () noexcept {
            auto it = impl.root();
            auto h = height;

            while (it != impl.end()) {
                ++it->childCnt;
                --h;

                if (it.left() == impl.end())
                    return it.left();

                if (is_complete (it.left(), h)) {
                    it.to_right();
                } else {
                    it.to_left();
                }
            }

            return it;
        }

        ConstNodeIt deleteFromOneOrTwo() noexcept {
            --height;

            if (height == 1) {
                --impl.root()->childCnt;
                return impl.root().left();
            }

            return impl.root();
        }

        ConstNodeIt deleteFromComplete() noexcept {
            auto it = impl.root();
            auto prevIt = it;

            do {
                --it->childCnt;
                prevIt = it;
                it.to_right();
            } while (it != impl.end());

            return prevIt;
        }

        ConstNodeIt deleteFromIncomplete() noexcept {
            auto it = impl.root();
            auto h = height;

             while (h != 2) {
                --h;
                --it->childCnt;

                if (is_complete (it.right(), h-1)) {
                    it.to_left();
                } else {
                    it.to_right();
                }
            }

            if (is_complete (impl.root(), height-1)) {
                --height;
            }

            return it.right() != impl.end()
                 ? it.right()
                 : it.left();
        }

        ConstNodeIt deletePosition() noexcept {
            if (size() <= 2)
                return deleteFromOneOrTwo();

            if (is_complete (impl.root(), height))
                return deleteFromComplete();

            return deleteFromIncomplete();
        }

    private:       
        Impl impl{};
        std::size_t height = 0;
    };
}

#endif