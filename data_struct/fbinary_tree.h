#ifndef FBINARY_TREE_H_GUARD
#define FBINARY_TREE_H_GUARD

#include "iterators/iterators_general.h"

namespace data_struct
{
    template <typename T, typename Container>
    class FBinTree {
        friend Container;

        struct Node {
            Node* left;
            Node* right;
            T value;
        };

        template <typename Mut>
        class BranchIterator {
            using Self = BranchIterator;
            using Ptr = Node*;
            using ReturnType = std::conditional_t<
                std::is_same_v<Mut, Mutable_tag>
              , T
              , T const
            >;

            template <typename Mut_>
            using EnableIfMutable = std::enable_if_t<std::is_same_v<Mut_, Mutable_tag>>;

            friend Container;
            friend FBinTree;
            friend BranchIterator<Const_tag>;

        public:
            using iterator_category = void;
            using difference_type   = void;

            using value_type = T;
            using reference  = ReturnType&;
            using pointer    = ReturnType*;
        
        public:
            BranchIterator() noexcept = default;
            BranchIterator (Self const&) noexcept = default;
            Self& operator= (Self const&) noexcept = default;
            ~BranchIterator() noexcept = default;

            BranchIterator (Self&& rhs) noexcept
                : ptrPtr (std::exchange (rhs.ptrPtr, nullptr))
            {}

            Self& operator= (Self&& rhs) noexcept {
                ptrPtr = std::exchange (rhs.ptrPtr, nullptr);
                return *this;
            }

            BranchIterator (Ptr* ptr) noexcept
                : ptrPtr (ptr)
            {}

            template <typename Mut_, typename = EnableIfMutable<Mut_>>
            BranchIterator (BranchIterator<Mut_> const& it) noexcept
                : ptrPtr (it.ptrPtr)
            {}

            friend
            bool operator== (Self const& lhs, Self const& rhs) noexcept {
                if (lhs.is_end() or rhs.is_end())
                    return lhs.is_end() and rhs.is_end();

                return lhs.ptrPtr == rhs.ptrPtr;
            }

            friend
            bool operator!= (Self const& lhs, Self const& rhs) noexcept {
                return not (lhs == rhs);
            }

            reference operator*() const noexcept {
                return real()->value;
            }

            pointer operator->() const noexcept {
                return &operator*();
            }

            Self left() const noexcept {
                auto tmp = *this;
                tmp.to_left();
                return tmp;
            }

            Self right() const noexcept {
                auto tmp = *this;
                tmp.to_right();
                return tmp;
            }

            void to_left() noexcept {
                ptrPtr = & real()->left;
            }

            void to_right() noexcept {
                ptrPtr = & real()->right;
            }

        private:
            Ptr& real() const noexcept {
                return *ptrPtr;
            }

            bool is_end() const noexcept {
                return ptrPtr == nullptr or real() == nullptr;
            }
    
        private:
            Ptr* ptrPtr = nullptr;
        };
    
    public:
        using Iterator = BranchIterator<Mutable_tag>;
        using ConstIterator = BranchIterator<Const_tag>;
    
    public:
        FBinTree() noexcept = default;
        ~FBinTree() noexcept = default;

        FBinTree& operator= (FBinTree&&) = default;
        FBinTree& operator= (FBinTree const&) = default;

        FBinTree (FBinTree&& rhs) noexcept
            : ptrRoot (std::exchange (rhs.ptrRoot, nullptr))
        {}

        FBinTree (FBinTree const& rhs) noexcept
            : ptrRoot (nullptr)
        {}

        bool empty() const noexcept {
            return ptrRoot == nullptr;
        }

        auto root() noexcept {
            return Iterator (&ptrRoot);
        }

        auto croot() const noexcept {
            return ConstIterator (
                const_cast<Node**> (&ptrRoot)
            );
        }

        auto root() const noexcept {
            return croot();
        }

        auto end() noexcept {
            return Iterator {};
        }

        auto cend() const noexcept {
            return ConstIterator {};
        }

        auto end() const noexcept {
            return cend();
        }

        template <typename... Ts>
        void emplace_node (ConstIterator it, Ts&&... params) {
            it.real() = new Node {
                nullptr
              , nullptr
              , std::forward<Ts> (params)...
            };
        }

        void erase_node (ConstIterator it) noexcept {
            delete (it.real());
            it.real() = nullptr;
        }

    private:
        Node* ptrRoot = nullptr;
    };
}

#endif