// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <math/vector_3d.hpp>
#include <math/trig.hpp>

#include <vector>

namespace noggit
{
template <typename T>
class history
{
public:
    history() : _pos(0), can_undo(false), can_redo(false)
    {
    }

    T* undo()
    {
        if (_container.empty())
            return nullptr;

        if (!can_undo)
            return nullptr;

        if (!_pos && can_undo)
            can_undo = false;

        can_redo = true;
        return &_container[_pos ? _pos-- : _pos];
    }

    T* redo()
    {
        if (_container.empty())
            return nullptr;

        if (!can_redo)
            return nullptr;

        if (_pos == (_container.size() - 1) && can_redo)
            can_redo = false;

        can_undo = true;
        return &_container[_pos != (_container.size() - 1) ? _pos++ : _pos];
    }

    void add(T elem)
    {
        if (!_container.empty() && _pos != (_container.size() - 1))
            _container.erase(_container.begin() + _pos + 1, _container.end());

        can_undo = true;
        can_redo = false;

        _container.push_back(elem);
        _pos = _container.size() - 1;
    }

    void clear()
    {
        _container.clear();
        _pos = 0;
    }

private:
    std::vector<T> _container;
    std::size_t _pos;

    bool can_undo;
    bool can_redo;
};
}