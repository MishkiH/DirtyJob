#pragma once

class Component {
public:
    virtual ~Component() = default;
    virtual void update() {}
    virtual void initialize() {}
};