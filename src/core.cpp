std::vector<unsigned char> Core::get_rendered_pixels() {
    return _renderer.get_framebuffer_pixels(_config.width, _config.height);
}