#ifndef _MULTI_ARRAY_H_
#define _MULTI_ARRAY_H_

#include <algorithm>
#include <cassert>
#include <type_traits>
#include "data/vec3.h"

namespace Coffee {

/// The multi_array class is a unified interface for 1D, 2D and 3D
/// arrays with proper index access and memory management. The most
/// benefit is indexing convenience. One-liners are just implemented in
/// the definition. Other functions are implemented in the accompanying
/// source file
template <typename T>
class multi_array {
 public:
  typedef multi_array<T> self_type;

  /// Default constructor, initializes `m_size` to zero and data pointers to
  /// `nullptr`.
  multi_array();

  /// Main constructor, initializes with given width, height, and
  /// depth of the array. Allocate memory in the initialization.
  explicit multi_array(int width, int height = 1, int depth = 1);

  /// Alternative main constructor, takes in an @ref Extent object and
  /// initializes an array of the corresponding extent.
  explicit multi_array(const Extent& extent);

  /// Standard copy constructor.
  multi_array(const self_type& other);

  /// Standard move constructor.
  multi_array(self_type&& other);

  /// Destructor. Delete the member data array.
  ~multi_array();

  /// Assignment operators for copying
  self_type& operator=(const self_type& other);

  /// Move assignment operator
  self_type& operator=(self_type&& other);

  /// Vector indexing operator, read only
  const T& operator()(int x, int y = 0, int z = 0) const;

  /// Vector indexing operator, read/write
  T& operator()(int x, int y = 0, int z = 0);

  /// Vector indexing operator using an @ref Index object, read only
  const T& operator()(const Index& index) const;

  /// Vector indexing operator using an @ref Index object, read and
  /// write
  T& operator()(const Index& index);

  /// Copying the entire content from another vector
  void copy_from(const self_type& other);

  /// Set the whole array to a single initial value
  void assign(const T& value);

  /// Resize the array.
  void resize(int width, int height = 1, int depth = 1);

  /// Resize the array according to an \ref Extent object.
  void resize(Extent extent);

  /// Copy the content from host to device
  void sync_to_device();

  /// Copy the content from device to host
  void sync_to_host();

  // Returns various sizes of the array
  int width() const { return m_extent.width(); }
  int height() const { return m_extent.height(); }
  int depth() const { return m_extent.depth(); }
  size_t size() const { return m_size; }
  const Extent& extent() const { return m_extent; }

  // Direct access to the encapsulated pointer
  void* host_ptr() { return m_data_h; }
  const void* host_ptr() const { return m_data_h; }
  void* dev_ptr() { return m_data_d; }
  const void* dev_ptr() const { return m_data_d; }

 private:
  void alloc_mem(size_t size);

  void free_mem();

  T* m_data_h; ///< Host data pointer
  T* m_data_d; ///< Device data pointer

  Extent m_extent;  ///< 3D extent of the array
  size_t m_size;  ///< Total size of the array
};

}  // namespace Coffee

#endif  // _MULTI_ARRAY_H_