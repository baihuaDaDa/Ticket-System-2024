/**
 * implement a container like std::set
 */
#ifndef TICKET_SYSTEM_SET_HPP
#define TICKET_SYSTEM_SET_HPP


// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace baihua {

    template<
            class T,
            class Compare = std::less<T>
    >
    class set {
    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::set as value_type by typedef.
         */
        typedef T value_type;

        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = set.begin(); --it;
         *       or it = set.end(); ++end();
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

        // 在接下来的rotate、insert和remove操作中，可能会对根节点有修改，需要同步修改set类中的成员root
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
            } else if (Compare()(node->data, elem)) {
                ret = Insert(elem, node->right, node, true);
                if (Height(node->right) - Height(node->left) == 2) {
                    if (Height(node->right->left) < Height(node->right->right))
                        RR(node);
                    else RL(node);
                }
            } else if (Compare()(elem, node->data)) {
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

        bool Remove(const value_type &elem, Node *node) {
            bool ret;
            if (node == nullptr) return true;
            if (Compare()(elem, node->data)) {
                if (Remove(elem, node->left)) ret = true;
                else ret = Adjust(node, false);
            } else if (Compare()(node->data, elem)) {
                if (Remove(elem, node->right)) ret = true;
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

            Node *node;
            const set *tag;

        public:
            iterator() {
                node = nullptr;
                tag = nullptr;
            }

            iterator(const iterator &other) {
                node = other.node;
                tag = other.tag;
            }

            explicit iterator(Node *_node, const set *_tag) : node(_node), tag(_tag) {}

            iterator operator++(int) {
                iterator tmp(*this);
                ++(*this);
                return tmp;
            }

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

            iterator operator--(int) {
                iterator tmp(*this);
                --(*this);
                return tmp;
            }

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

            value_type &operator*() const {
                return node->data;
            }

            bool operator==(const iterator &rhs) const {
                return node == rhs.node && tag == rhs.tag;
            }

            bool operator==(const const_iterator &rhs) const {
                return node == rhs.node && tag == rhs.tag;
            }

            bool operator!=(const iterator &rhs) const {
                return node != rhs.node || tag != rhs.tag;
            }

            bool operator!=(const const_iterator &rhs) const {
                return node != rhs.node || tag != rhs.tag;
            }

            friend const_iterator;
            friend set;
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
            const set *tag;

        public:
            const_iterator() {
                node = nullptr;
                tag = nullptr;
            }

            const_iterator(const const_iterator &other) {
                node = other.node;
                tag = other.tag;
            }

            const_iterator(const iterator &other) {
                node = other.node;
                tag = other.tag;
            }

            explicit const_iterator(const Node *_node, const set *_tag) : node(_node), tag(_tag) {}

            const_iterator operator++(int) {
                const_iterator tmp(*this);
                ++(*this);
                return tmp;
            }

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

            const_iterator operator--(int) {
                const_iterator tmp(*this);
                --(*this);
                return tmp;
            }

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

            const value_type &operator*() const {
                return node->data;
            }

            bool operator==(const iterator &rhs) const {
                return node == rhs.node && tag == rhs.tag;
            }

            bool operator==(const const_iterator &rhs) const {
                return node == rhs.node && tag == rhs.tag;
            }

            bool operator!=(const iterator &rhs) const {
                return node != rhs.node || tag != rhs.tag;
            }

            bool operator!=(const const_iterator &rhs) const {
                return node != rhs.node || tag != rhs.tag;
            }

            friend set;
        };

        set() : root(nullptr), _size(0), _begin(iterator(nullptr, this)), _end_(iterator(nullptr, this)), _end(iterator(nullptr, this)) {}

        set(const set &other) : _size(other._size), _end(iterator(nullptr, this)) {
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

        set &operator=(const set &other) {
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

        ~set() {
            Clear(root);
        }

        iterator begin() {
            return _begin;
        }

        const_iterator cbegin() const {
            return const_iterator(_begin);
        }

        iterator end() {
            return _end;
        }

        const_iterator cend() const {
            return const_iterator(_end);
        }

        bool empty() const {
            return _size == 0;
        }

        size_t size() const {
            return _size;
        }

        void clear() {
            Clear(root);
            root = nullptr;
            _size = 0;
            _begin = _end_ = iterator(nullptr, this);
        }

        pair<iterator, bool> insert(const value_type &value) {
            size_t origin = _size;
            iterator iter = Insert(value, root, nullptr, false);
            if (origin != _size) {
                if (_begin.node == nullptr) _begin = _end_ = iter;
                else {
                    if (Compare()(*iter, *_begin))
                        _begin = iter;
                    if (Compare()(*_end_, *iter))
                        _end_ = iter;
                }
            }
            return (origin == _size) ? pair<iterator, bool>(iter, false) : pair<iterator, bool>(iter, true);
        }

        void erase(iterator pos) {
            if (pos.node == nullptr || pos.tag != this) throw invalid_iterator();
            bool if_begin_changed = false, if__end_changed = false;
            if (pos == _begin) if_begin_changed = true;
            if (pos == _end_) if__end_changed = true;
            Remove(pos.node->data, root);
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

        size_t count(const value_type &elem) const {
            const_iterator iter = find(elem);
            return (iter.node == nullptr) ? 0 : 1;
        }

        iterator find(const value_type &elem) {
            Node *now = root;
            while (now != nullptr) {
                if (Compare()(now->data, elem)) now = now->right;
                else if (Compare()(elem, now->data)) now = now->left;
                else return iterator(now, this);
            }
            return iterator(nullptr, this);
        }

        const_iterator find(const value_type &elem) const {
            const Node *now = root;
            while (now != nullptr) {
                if (Compare()(now->data, elem)) now = now->right;
                else if (Compare()(elem, now->data)) now = now->left;
                else return const_iterator(now, this);
            }
            return const_iterator(nullptr, this);
        }

//        void traverse() {
//            std::queue<Node *> queue;
//            queue.push(root);
//            std::cout << (root == nullptr ? 0 : root->data) << ' ';
//            if (root == nullptr) queue.pop();
//            while (!queue.empty()) {
//                Node *tmp = queue.front();
//                queue.pop();
//                if (tmp->left == nullptr) std::cout << 0 << ' ';
//                else {
//                    std::cout << tmp->left->data << ' ' << tmp->left->height << ' ';
//                    queue.push(tmp->left);
//                }
//                if (tmp->right == nullptr) std::cout << 0 << ' ';
//                else {
//                    std::cout << tmp->right->data << ' ' << tmp->right->height << ' ';
//                    queue.push(tmp->right);
//                }
//
//            }
//            std::cout << std::endl;
//        }
    };

}


#endif //TICKET_SYSTEM_SET_HPP
