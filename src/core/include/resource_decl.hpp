#ifndef physp_core_resource_decl_hpp
#define physp_core_resource_decl_hpp

#include "src/core/include/allocator.hpp"

namespace pp
{

enum ResourceType
{
  vertex,
  index,
  uniform,
  storage
};

class IResource
{
  friend class Allocation;
  friend class Allocator;

  public:
    IResource(std::size_t);
    IResource(const IResource&);
    IResource(IResource&&);

    virtual ~IResource();

    IResource& operator = (const IResource&);
    IResource& operator = (IResource&&);

  public:
    ResourceType resource_type = uniform;
    bool is_shared = false;
    const std::size_t size;
    Allocator::AllocationIndex allocation_index;

  protected:
    Allocator * p_allocator = nullptr;
    Buffer * p_buffer = nullptr;

  private:
    std::mutex parent_mutex;
};

template <typename T>
class Resource : public IResource
{
  public:
    Resource(T, ResourceType t = uniform, bool s = false);
    Resource(const Resource&) = default;
    Resource(Resource&&) = default;

    ~Resource() = default;

    Resource& operator = (const Resource&);
    Resource& operator = (Resource&&);
    Resource& operator = (T&);
    Resource& operator = (T&&);
    const T& operator * () const;

    const vk::raii::Buffer& buffer() const;

  private:
    void updateAllocation() const;

  private:
    T data;
    std::mutex data_mutex;
};

} // namespace pp

#endif // physp_core_resource_decl_hpp