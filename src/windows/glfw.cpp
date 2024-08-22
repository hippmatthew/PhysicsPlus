#include "src/core/include/settings.hpp"
#include "vulkan/vulkan_core.h"
#include "src/windows/include/glfw.hpp"

namespace pp::windows
{

GLFW::GLFW()
{
  auto& s_window = pp_settings_manager.settings<Window>();

  if (!s_window.initialized)
  {
    glfwInit();
    s_window.initialized = true;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  gl_window = glfwCreateWindow(
    s_window.size[0],
    s_window.size[1],
    s_window.title.c_str(),
    nullptr, nullptr
  );

  float sx, sy;
  glfwGetWindowContentScale(gl_window, &sx, &sy);

  s_window.scale = { sx, sy };
}

GLFW::~GLFW()
{
  glfwDestroyWindow(gl_window);
  glfwTerminate();
}

std::vector<const char *> GLFW::instance_extensions()
{
  auto& s_window = pp_settings_manager.settings<Window>();
  if (!s_window.initialized)
  {
    glfwInit();
    s_window.initialized = true;
  }

  unsigned int count;
  const char ** extensions = glfwGetRequiredInstanceExtensions(&count);

  return std::vector<const char *>(extensions, extensions + count);
}

std::vector<const char *> GLFW::device_extensions()
{
  return { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}

bool GLFW::should_close() const
{
  return glfwWindowShouldClose(gl_window);
}

void GLFW::poll_events() const
{
  glfwPollEvents();
}

void GLFW::create_surface(const vk::raii::Instance& vk_instance)
{
  VkSurfaceKHR surface = nullptr;
  glfwCreateWindowSurface(*vk_instance, gl_window, nullptr, &surface);

  vk_surface = vk::raii::SurfaceKHR(vk_instance, surface);
}

} // namespace pp::windows