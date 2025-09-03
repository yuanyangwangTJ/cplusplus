#include <cstddef>
#include <memory>

class MyLinkedList {
public:
  MyLinkedList() {
    size_ = 0;
    dummy_ = std::make_shared<ListNode>(0, nullptr);
  }

  int get(int index) {
    if (index < 0 || index >= size_) {
      return -1;
    }
    auto curr = dummy_;
    for (int i = 0; i <= index; i++) {
      curr = curr->next;
    }
    return curr->val;
  }

  void addAtHead(int val) {
    auto newNode = std::make_shared<ListNode>(val, dummy_->next);
    dummy_->next = newNode;
    size_++;
  }

  void addAtTail(int val) {
    auto curr = dummy_;
    while (curr->next) {
      curr = curr->next;
    }
    curr->next = std::make_shared<ListNode>(val, nullptr);
    size_++;
  }

  void addAtIndex(int index, int val) {
    if (index < 0 || index > size_) {
      return;
    }
    auto curr = dummy_;
    for (int i = 0; i < index; i++) {
      curr = curr->next;
    }
    auto newNode = std::make_shared<ListNode>(val, curr->next);
    curr->next = newNode;
    size_++;
  }

  void deleteAtIndex(int index) {
    if (index < 0 || index >= size_) {
      return;
    }
    auto curr = dummy_;
    for (int i = 0; i < index; i++) {
      curr = curr->next;
    }
    curr->next = curr->next->next;
    size_--;
  }

private:
  struct ListNode {
    int val;
    std::shared_ptr<ListNode> next;
    ListNode(int v, std::shared_ptr<ListNode> next_node)
        : val(v), next(next_node) {}
  };
  std::shared_ptr<ListNode> dummy_;
  size_t size_ = 0;
};

/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->addAtHead(val);
 * obj->addAtTail(val);
 * obj->addAtIndex(index,val);
 * obj->deleteAtIndex(index);
 */

int main()
{

}