#ifndef BAIHUA_LIST_HPP
#define BAIHUA_LIST_HPP

#include <cstddef>
#include "exceptions.hpp"
#include <memory>

namespace baihua {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a
 * list.
 */
    template<typename T>
    class list {
    protected:
        class node {
        public:
            /**
             * add data members and constructors & destructor
             */
            node *pre;
            node *next;
            T data;

            node() : data(), pre(nullptr), next(nullptr) {}

            node(node *_pre, node *_next) : data(), pre(_pre), next(_next) {}

            node(const T &_data) : data(_data), pre(nullptr), next(nullptr) {}

            node(const T &_data, node *_pre, node *_next) : pre(_pre), next(_next), data(_data) {}
        };

    protected:
        /**
         * add data members for linked list as protected members
         */
        node *head;
        node *tail;
        size_t len;

        /**
         * insert node cur before node pos
         * return the inserted node cur
         */
        void insert(node *pos, node *cur) {
            cur->next = pos;
            cur->pre = pos->pre;
            pos->pre->next = cur;
            pos->pre = cur;
            len++;
        }

        /**
         * remove node pos from list (no need to delete the node)
         * return the removed node pos
         */
        void erase(node *pos) {
            pos->pre->next = pos->next;
            pos->next->pre = pos->pre;
            pos->pre = nullptr;
            pos->next = nullptr;
            len--;
        }

    public:
        class const_iterator;

        class iterator {
            friend const_iterator;
            friend list<T>;
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            node *pos;

        public:
            iterator() : pos(nullptr) {}

            iterator(node *_pos) : pos(_pos) {}

            iterator operator++(int n) {
                iterator origin = *this;
                pos = pos->next;
                return origin;
            }

            iterator &operator++() {
                pos = pos->next;
                return *this;
            }

            iterator operator--(int n) {
                iterator origin = *this;
                pos = pos->pre;
                return origin;
            }

            iterator &operator--() {
                pos = pos->pre;
                return *this;
            }

            /**
             * TODO *it
             * throw std::exception if iterator is invalid
             */
            T &operator*() const {
                if (pos == nullptr || pos->pre == nullptr) throw invalid_iterator();
                return pos->data;
            }

            /**
             * TODO it->field
             * throw std::exception if iterator is invalid
             */
            T *operator->() const noexcept {
                if (pos == nullptr || pos->pre == nullptr) throw invalid_iterator();
                return &(pos->data);
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same
             * memory).
             */
            bool operator==(const iterator &rhs) const {
                return pos == rhs.pos;
            }

            bool operator==(const const_iterator &rhs) const {
                return pos == rhs.pos;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return pos != rhs.pos;
            }

            bool operator!=(const const_iterator &rhs) const {
                return pos != rhs.pos;
            }
        };

        /**
         * TODO
         * has same function as iterator, just for a const object.
         * should be able to construct from an iterator.
         */
        class const_iterator {
            friend list<T>;
        private:
            const node *pos;

        public:
            const_iterator() : pos(nullptr) {}

            const_iterator(node *_pos) : pos(_pos) {}

            const_iterator(const iterator &iter) {
                pos = iter.pos;
            }

            const_iterator operator++(int n) {
                const_iterator origin = *this;
                pos = pos->next;
                return origin;
            }

            const_iterator &operator++() {
                pos = pos->next;
                return *this;
            }

            const_iterator operator--(int n) {
                const_iterator origin = *this;
                pos = pos->pre;
                return origin;
            }

            const_iterator &operator--() {
                pos = pos->pre;
                return *this;
            }

            const T &operator*() const {
                if (pos == nullptr || pos->pre == nullptr) throw invalid_iterator();
                return pos->data;
            }

            const T *operator->() const noexcept {
                if (pos == nullptr || pos->pre == nullptr) throw invalid_iterator();
                return &(pos->data);
            }

            bool operator==(const iterator &rhs) const {
                return pos == rhs.pos;
            }

            bool operator==(const const_iterator &rhs) const {
                return pos == rhs.pos;
            }

            bool operator!=(const iterator &rhs) const {
                return pos != rhs.pos;
            }

            bool operator!=(const const_iterator &rhs) const {
                return pos != rhs.pos;
            }
        };

        /**
         * TODO Constructs
         * At least two: default constructor, copy constructor
         */
        list() : head(new node()), tail(new node()), len(0) {
            head->next = tail;
            tail->pre = head;
        }

        list(const list &other) : list() {
            node *pos = other.head->next;
            while (pos != other.tail) {
                node *new_node = new node(pos->data);
                insert(tail, new_node);
                pos = pos->next;
            }
        }

        /**
         * TODO Destructor
         */
        virtual ~list() {
            this->clear();
            delete head;
            delete tail;
        }

        /**
         * TODO Assignment operator
         */
        list &operator=(const list &other) {
            if (this == &other) return *this;
            clear();
            node *pos = other.head->next;
            while (pos != other.tail) {
                node *new_node = new node(pos->data);
                insert(tail, new_node);
                pos = pos->next;
            }
            return *this;
        }

        /**
         * access the first / last element
         * throw container_is_empty when the container is empty.
         */
        const T &front() const {
            if (this->empty()) throw container_is_empty();
            return head->next->data;
        }

        const T &back() const {
            if (this->empty()) throw container_is_empty();
            return tail->pre->data;
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            iterator iter(head->next);
            return iter;
        }

        const_iterator cbegin() const {
            const_iterator const_iter(head->next);
            return const_iter;
        }

        /**
         * returns an iterator to the end.
         */
        iterator end() {
            iterator iter(tail);
            return iter;
        }

        const_iterator cend() const {
            const_iterator const_iter(tail);
            return const_iter;
        }

        /**
         * checks whether the container is empty.
         */
        virtual bool empty() const {
            return len == 0;
        }

        /**
         * returns the number of elements
         */
        virtual size_t size() const {
            return len;
        }

        /**
         * clears the contents
         */
        virtual void clear() {
            node *pos = head->next;
            while (pos != tail) {
                node *to_be_deleted = pos;
                pos = pos->next;
                delete to_be_deleted;
            }
            head->next = tail;
            tail->pre = head;
            len = 0;
        }

        /**
         * insert value before pos (pos may be the end() iterator)
         * return an iterator pointing to the inserted value
         * throw if the iterator is invalid
         */
        virtual iterator insert(iterator pos, const T &value) {
            if (pos.pos == nullptr || pos.pos->pre == nullptr)
                throw invalid_iterator();
            node *new_node = new node(value);
            insert(pos.pos, new_node);
            return --pos;
        }

        /**
         * remove the element at pos (the end() iterator is invalid)
         * returns an iterator pointing to the following element, if pos pointing to
         * the last element, end() will be returned. throw if the container is empty,
         * the iterator is invalid
         */
        virtual iterator erase(iterator pos) {
            if (this->empty() || pos.pos == nullptr || pos == this->end() || pos.pos->pre == nullptr)
                throw invalid_iterator();
            iterator next_iter = iterator(pos.pos->next);
            erase(pos.pos);
            delete pos.pos;
            return next_iter;
        }

        /**
         * adds an element to the end
         */
        void push_back(const T &value) {
            node *new_node = new node(value, tail->pre, tail);
            tail->pre->next = new_node;
            tail->pre = new_node;
            len++;
        }

        /**
         * removes the last element
         * throw when the container is empty.
         */
        void pop_back() {
            if (this->empty()) throw invalid_iterator();
            node *to_be_deleted = tail->pre;
            tail->pre = to_be_deleted->pre;
            tail->pre->next = tail;
            delete to_be_deleted;
            len--;
        }

        /**
         * inserts an element to the beginning.
         */
        void push_front(const T &value) {
            node *new_node = new node(value, head, head->next);
            head->next->pre = new_node;
            head->next = new_node;
            len++;
        }

        /**
         * removes the first element.
         * throw when the container is empty.
         */
        void pop_front() {
            if (this->empty()) throw invalid_iterator();
            node *to_be_deleted = head->next;
            head->next = to_be_deleted->next;
            head->next->pre = head;
            delete to_be_deleted;
            len--;
        }
    };

} // namespace sjtu

#endif // BAIHUA_LIST_HPP
