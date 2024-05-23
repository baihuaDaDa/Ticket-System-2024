/**
 * implement a container like std::map
 */
#ifndef BAIHUA_MAP_HPP
#define BAIHUA_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace baihua {

    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    >
    class map {
    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;

        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
        class const_iterator;

        class iterator;

    private:
        struct Node {
            value_type data;
            size_t height;
            Node *left;
            Node *right;
            Node *father;

            Node(const value_type &elem) : data(elem), height(1), left(nullptr), right(nullptr), father(nullptr) {}

            Node(const value_type &elem, size_t _height) : data(elem), height(_height), left(nullptr), right(nullptr),
                                                           father(nullptr) {}

            Node(const value_type &elem, Node *_father) : data(elem), height(1), left(nullptr),
                                                          right(nullptr), father(_father) {}

            Node(const value_type &elem, size_t _height, Node *_left, Node *_right, Node *_father)
                    : data(elem), height(_height), left(_left), right(_right), father(_father) {}
        };

        Node *root;
        size_t _size;
        iterator _begin;
        iterator _end;
        iterator _end_;

        // delete之后由于还有可能调用，需要将被delete的node置成nullptr

        Node *BuildTree(const Node *const &other, Node *father) {
            if (other == nullptr) return nullptr;
            Node *node = new Node(other->data, other->height);
            node->left = BuildTree(other->left, node);
            node->right = BuildTree(other->right, node);
            node->father = father;
            return node;
        }

        void Clear(Node *&node) {
            if (node == nullptr) return;
            Clear(node->left);
            Clear(node->right);
            delete node;
        }

        size_t Height(const Node *const &node) const {
            return node == nullptr ? 0 : node->height;
        }

        // 在接下来的rotate、insert和remove操作中，可能会对根节点有修改，需要同步修改map类中的成员root
        // 为了防止搞不清同步改了什么指针，我选择避免使用引用而直接传递值，在函数内另外修改对应的三叉链表的三条双向边
        // 有时候可能会访问到根节点，而根节点的父节点是nullptr，这时需要特判

        void LL(Node *node) {
            if (node->father != nullptr) {
                if (node->father->left == node) node->father->left = node->left;
                else node->father->right = node->left;
            } else root = node->left;
            Node *_right = node;
            node = node->left;
            node->father = _right->father;
            _right->left = node->right;
            if (node->right != nullptr) node->right->father = _right;
            _right->father = node;
            node->right = _right;
            _right->height = std::max(Height(_right->left), Height(_right->right)) + 1;
            node->height = std::max(Height(node->left), Height(_right)) + 1;
        }

        void RR(Node *node) {
            if (node->father != nullptr) {
                if (node->father->left == node) node->father->left = node->right;
                else node->father->right = node->right;
            } else root = node->right;
            Node *_left = node;
            node = node->right;
            node->father = _left->father;
            _left->right = node->left;
            if (node->left != nullptr) node->left->father = _left;
            _left->father = node;
            node->left = _left;
            _left->height = std::max(Height(_left->left), Height(_left->right)) + 1;
            node->height = std::max(Height(node->right), Height(_left)) + 1;
        }

        void LR(Node *node) {
            RR(node->left);
            LL(node);
        }

        void RL(Node *node) {
            LL(node->right);
            RR(node);
        }

        // 小小疑问：为什么在调试过程中发现这里的node被旋转之后会自己改成新的根节点？明明这里的rotate函数时值传递啊

        iterator Insert(const value_type &elem, Node *&node, Node *const &father, bool subtree) {
            iterator ret;
            if (node == nullptr) {
                node = new Node(elem, father);
                if (father != nullptr) {
                    if (subtree) father->right = node;
                    else father->left = node;
                }
                _size++;
                ret = iterator(node, this);
            } else if (Compare()(node->data.first, elem.first)) {
                ret = Insert(elem, node->right, node, true);
                if (Height(node->right) - Height(node->left) == 2) {
                    if (Height(node->right->left) < Height(node->right->right))
                        RR(node);
                    else RL(node);
                }
            } else if (Compare()(elem.first, node->data.first)) {
                ret = Insert(elem, node->left, node, false);
                if (Height(node->left) - Height(node->right) == 2) {
                    if (Height(node->left->left) < Height(node->left->right))
                        LR(node);
                    else LL(node);
                }
            } else ret = iterator(node, this);
            node->height = std::max(Height(node->left), Height(node->right)) + 1;
            return ret;
        }

        // 这里需要同步修改node，因为在remove函数中node可能会在adjust之后被用到

        bool Adjust(Node *&node, bool subtree) {
            if (subtree) {
                if (Height(node->left) - Height(node->right) == 1) return true;
                if (Height(node->left) - Height(node->right) == 0) {
                    node->height--;
                    return false;
                }
                if (Height(node->left->right) > Height(node->left->left)) {
                    LR(node);
                    node = node->father;
                    return false;
                }
                LL(node);
                node = node->father;
                if (Height(node->left) == Height(node->right)) return false;
                else return true;
            } else {
                if (Height(node->right) - Height(node->left) == 1) return true;
                if (Height(node->right) - Height(node->left) == 0) {
                    node->height--;
                    return false;
                }
                if (Height(node->right->left) > Height(node->right->right)) {
                    RL(node);
                    node = node->father;
                    return false;
                }
                RR(node);
                node = node->father;
                if (Height(node->left) == Height(node->right)) return false;
                else return true;
            }
        }

        // 感觉可以写的更好看一点，在Node里存一个左右子树tag

        bool Remove(const Key &key, Node *node) {
            bool ret;
            if (node == nullptr) return true;
            if (Compare()(key, node->data.first)) {
                if (Remove(key, node->left)) ret = true;
                else ret = Adjust(node, false);
            } else if (Compare()(node->data.first, key)) {
                if (Remove(key, node->right)) ret = true;
                else ret = Adjust(node, true);
            } else {
                if (node->left == nullptr && node->right == nullptr) {
                    _size--;
                    Node *to_be_deleted = node;
                    if (node->father != nullptr) {
                        if (node->father->left == node) node->father->left = nullptr;
                        else node->father->right = nullptr;
                    }
                    delete to_be_deleted;
                    if (_size == 0) root = nullptr;
                    return false;
                } else if (node->left == nullptr || node->right == nullptr) {
                    _size--;
                    Node *to_be_deleted = node;
                    node = (node->left == nullptr) ? node->right : node->left;
                    if (to_be_deleted->father != nullptr) {
                        if (to_be_deleted->father->left == to_be_deleted) to_be_deleted->father->left = node;
                        else to_be_deleted->father->right = node;
                    } else root = node;
                    node->father = to_be_deleted->father;
                    delete to_be_deleted;
                    ret = false;
                } else {
                    // 这里要注意除了被删除的元素对应的迭代器，其他迭代器在删除之后不受影响，所以所有元素的地址不能变化，也就是说不能新建node
                    --_size;
                    Node *tmp = node->right;
                    while (tmp->left != nullptr)
                        tmp = tmp->left;
                    bool lr = (tmp->father->left != tmp);
                    if (tmp->right == nullptr) {
                        if (!lr) tmp->father->left = nullptr;
                        else tmp->father->right = nullptr;
                    } else {
                        tmp->right->father = tmp->father;
                        if (!lr) tmp->father->left = tmp->right;
                        else tmp->father->right = tmp->right;
                    }
                    Node *to_be_adjusted = (tmp->father != node) ? tmp->father : tmp;
                    if (node->father != nullptr) {
                        if (node->father->left == node) node->father->left = tmp;
                        else node->father->right = tmp;
                    }
                    tmp->father = node->father;
                    tmp->left = node->left;
                    tmp->right = node->right;
                    tmp->height = node->height;
                    node->left->father = tmp;
                    if (node->right != nullptr) node->right->father = tmp;
                    delete node;
                    while (to_be_adjusted != tmp) {
                        if (Adjust(to_be_adjusted, lr)) {
                            ret = true;
                            break;
                        }
                        lr = to_be_adjusted->father->left != to_be_adjusted;
                        to_be_adjusted = to_be_adjusted->father;
                    }
                    if (to_be_adjusted == tmp)
                        ret = Adjust(tmp, true);
                    node = tmp;
                    if (node->father == nullptr) root = node;
                }
            }
            node->height = std::max(Height(node->right), Height(node->left)) + 1;
            return ret;
        }

    public:
        class iterator {
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            struct sorted_iterator_tag : public std::bidirectional_iterator_tag {};

        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            Node *node;
            const map *tag;

        public:
            iterator() {
                // TODO
                node = nullptr;
                tag = nullptr;
            }

            iterator(const iterator &other) {
                // TODO
                node = other.node;
                tag = other.tag;
            }

            explicit iterator(Node *_node, const map *_tag) : node(_node), tag(_tag) {}

            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator tmp(*this);
                ++(*this);
                return tmp;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++() {
                if (node == nullptr) throw invalid_iterator();
                if (node->right == nullptr) {
                    while (node->father != nullptr && node->father->right == node)
                        node = node->father;
                    if (node->father != nullptr) node = node->father;
                    else node = nullptr;
                    return *this;
                }
                node = node->right;
                while (node->left != nullptr)
                    node = node->left;
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp(*this);
                --(*this);
                return tmp;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                if (node == nullptr) {
                    if (tag->empty()) throw invalid_iterator();
                    else {
                        node = tag->_end_.node;
                        return *this;
                    }
                }
                if (node->left == nullptr) {
                    while (node->father != nullptr && node->father->left == node)
                        node = node->father;
                    if (node->father != nullptr) {
                        node = node->father;
                        return *this;
                    } else throw invalid_iterator();
                }
                node = node->left;
                while (node->right != nullptr)
                    node = node->right;
                return *this;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type &operator*() const {
                return node->data;
            }

            bool operator==(const iterator &rhs) const {
                return node == rhs.node && tag == rhs.tag;
            }

            bool operator==(const const_iterator &rhs) const {
                return node == rhs.node && tag == rhs.tag;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return node != rhs.node || tag != rhs.tag;
            }

            bool operator!=(const const_iterator &rhs) const {
                return node != rhs.node || tag != rhs.tag;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type *operator->() const noexcept {
                return &node->data;
            }

            friend const_iterator;
            friend map;
        };

        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            struct sorted_iterator_tag : public std::bidirectional_iterator_tag {};

        private:
            // data members.
            const Node *node;
            const map *tag;

        public:
            const_iterator() {
                // TODO
                node = nullptr;
                tag = nullptr;
            }

            const_iterator(const const_iterator &other) {
                // TODO
                node = other.node;
                tag = other.tag;
            }

            const_iterator(const iterator &other) {
                // TODO
                node = other.node;
                tag = other.tag;
            }

            explicit const_iterator(const Node *_node, const map *_tag) : node(_node), tag(_tag) {}

            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                const_iterator tmp(*this);
                ++(*this);
                return tmp;
            }

            /**
             * TODO ++iter
             */
            const_iterator &operator++() {
                if (node == nullptr) throw invalid_iterator();
                if (node->right == nullptr) {
                    while (node->father != nullptr && node->father->right == node)
                        node = node->father;
                    if (node->father != nullptr) node = node->father;
                    else node = nullptr;
                    return *this;
                }
                node = node->right;
                while (node->left != nullptr)
                    node = node->left;
                return *this;
            }

            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator tmp(*this);
                --(*this);
                return tmp;
            }

            /**
             * TODO --iter
             */
            const_iterator &operator--() {
                if (node == nullptr) {
                    if (tag->empty()) throw invalid_iterator();
                    else {
                        node = tag->_end_.node;
                        return *this;
                    }
                }
                if (node->left == nullptr) {
                    while (node->father != nullptr && node->father->left == node)
                        node = node->father;
                    if (node->father != nullptr) {
                        node = node->father;
                        return *this;
                    } else throw invalid_iterator();
                }
                node = node->left;
                while (node->right != nullptr)
                    node = node->right;
                return *this;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            const value_type &operator*() const {
                return node->data;
            }

            bool operator==(const iterator &rhs) const {
                return node == rhs.node && tag == rhs.tag;
            }

            bool operator==(const const_iterator &rhs) const {
                return node == rhs.node && tag == rhs.tag;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return node != rhs.node || tag != rhs.tag;
            }

            bool operator!=(const const_iterator &rhs) const {
                return node != rhs.node || tag != rhs.tag;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            const value_type *operator->() const noexcept {
                return &node->data;
            }

            friend map;
        };

        /**
         * TODO two constructors
         */
        map() : root(nullptr), _size(0), _begin(iterator(nullptr, this)), _end_(iterator(nullptr, this)), _end(iterator(nullptr, this)) {}

        map(const map &other) : _size(other._size), _end(iterator(nullptr, this)) {
            if (other.root == nullptr) {
                root = nullptr;
                _begin = _end_ = iterator(nullptr, this);
            } else {
                root = new Node(other.root->data, nullptr);
                root->height = other.root->height;
                root->left = BuildTree(other.root->left, root);
                root->right = BuildTree(other.root->right, root);
                _begin.tag = _end_.tag = this;
                _begin.node = root;
                while (_begin.node->left != nullptr)
                    _begin.node = _begin.node->left;
                _end_.node = root;
                while (_end_.node->right != nullptr)
                    _end_.node = _end_.node->right;
            }
        }

        /**
         * TODO assignment operator
         */
        map &operator=(const map &other) {
            if (&other == this) return *this;
            Clear(root);
            _size = other._size;
            if (other.root == nullptr) {
                root = nullptr;
                _begin = _end_ = iterator(nullptr, this);
            }
            else {
                root = new Node(other.root->data, nullptr);
                root->height = other.root->height;
                root->left = BuildTree(other.root->left, root);
                root->right = BuildTree(other.root->right, root);
                _begin.node = root;
                while (_begin.node->left != nullptr)
                    _begin.node = _begin.node->left;
                _end_.node = root;
                while (_end_.node->right != nullptr)
                    _end_.node = _end_.node->right;
            }
            return *this;
        }

        /**
         * TODO Destructors
         */
        ~map() {
            Clear(root);
        }

        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T &at(const Key &key) {
            iterator iter = find(key);
            if (iter.node == nullptr) throw index_out_of_bound();
            return iter->second;
        }

        const T &at(const Key &key) const {
            const_iterator iter = find(key);
            if (iter.node == nullptr) throw index_out_of_bound();
            return iter->second;
        }

        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T &operator[](const Key &key) {
            iterator iter = find(key);
            if (iter.node == nullptr) {
                iter = insert(value_type(key, T())).first;
                return iter->second;
            }
            return iter->second;
        }

        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T &operator[](const Key &key) const {
            return at(key);
        }

        /**
         * return a iterator to the beginning
         */
        iterator begin() {
            return _begin;
        }

        const_iterator cbegin() const {
            return const_iterator(_begin);
        }

        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {
            return _end;
        }

        const_iterator cend() const {
            return const_iterator(_end);
        }

        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {
            return _size == 0;
        }

        /**
         * returns the number of elements.
         */
        size_t size() const {
            return _size;
        }

        /**
         * clears the contents
         */
        void clear() {
            Clear(root);
            root = nullptr;
            _size = 0;
            _begin = _end_ = iterator(nullptr, this);
        }

        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value) {
            size_t origin = _size;
            iterator iter = Insert(value, root, nullptr, false);
            if (origin != _size) {
                if (_begin.node == nullptr) _begin = _end_ = iter;
                else {
                    if (Compare()(iter->first, _begin->first))
                        _begin = iter;
                    if (Compare()(_end_->first, iter->first))
                        _end_ = iter;
                }
            }
            return (origin == _size) ? pair<iterator, bool>(iter, false) : pair<iterator, bool>(iter, true);
        }

        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            if (pos.node == nullptr || pos.tag != this) throw invalid_iterator();
            bool if_begin_changed = false, if__end_changed = false;
            if (pos == _begin) if_begin_changed = true;
            if (pos == _end_) if__end_changed = true;
            Remove(pos.node->data.first, root);
            if (if_begin_changed) {
                if (root == nullptr) {
                    _begin = iterator(nullptr, this);
                } else {
                    _begin.node = root;
                    while (_begin.node->left != nullptr)
                        _begin.node = _begin.node->left;
                }
            }
            if (if__end_changed) {
                if (if_begin_changed) _end_ = _begin;
                else {
                    _end_.node = root;
                    while (_end_.node->right != nullptr)
                        _end_.node = _end_.node->right;
                }
            }
        }

        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const {
            const_iterator iter = find(key);
            return (iter.node == nullptr) ? 0 : 1;
        }

        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key) {
            Node *now = root;
            while (now != nullptr) {
                if (Compare()(now->data.first, key)) now = now->right;
                else if (Compare()(key, now->data.first)) now = now->left;
                else return iterator(now, this);
            }
            return iterator(nullptr, this);
        }

        const_iterator find(const Key &key) const {
            const Node *now = root;
            while (now != nullptr) {
                if (Compare()(now->data.first, key)) now = now->right;
                else if (Compare()(key, now->data.first)) now = now->left;
                else return const_iterator(now, this);
            }
            return const_iterator(nullptr, this);
        }

//        void traverse() {
//            std::queue<Node *> queue;
//            queue.push(root);
//            std::cout << (root == nullptr ? 0 : root->data.first) << ' ';
//            if (root == nullptr) queue.pop();
//            while (!queue.empty()) {
//                Node *tmp = queue.front();
//                queue.pop();
//                if (tmp->left == nullptr) std::cout << 0 << ' ';
//                else {
//                    std::cout << tmp->left->data.first << ' ' << tmp->left->height << ' ';
//                    queue.push(tmp->left);
//                }
//                if (tmp->right == nullptr) std::cout << 0 << ' ';
//                else {
//                    std::cout << tmp->right->data.first << ' ' << tmp->right->height << ' ';
//                    queue.push(tmp->right);
//                }
//
//            }
//            std::cout << std::endl;
//        }
    };

}

#endif
