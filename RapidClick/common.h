#pragma once

#include <memory>
#include <mutex>

#define PIMPL class Data; std::unique_ptr<Data> data; Data& d() { return *data; }; Data const& d() const { return *data; };