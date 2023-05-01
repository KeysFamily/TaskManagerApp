#include "stdafx.h"
#include "SivTask.hpp"

bool SivTask::isDead() {
    return dead;
}

bool SivTask::Kill() {
    this->dead = true;
    return true;
}

void SivEngine::TaskUpDate() {
    for (auto& o : task) {
        (*o).update();
        (*o).draw();
    }
}

void SivEngine::TaskAdd() {
    task.insert(task.end(), add_Tasks.begin(), add_Tasks.end());
    add_Tasks.clear(); //追加用は追加終わったら消す。
}

void SivEngine::TaskRemove() {
    task.erase(
        std::remove_if(std::begin(task), std::end(task), [](std::shared_ptr<SivTask> c) { return  c->isDead(); }),
        std::cend(task));
}



template <class T>    std::shared_ptr<T> SivEngine::GetTask(String gn_, String n_) {
    for (auto& o : task) {
        if (o->groupName != gn_) { continue; }
        if (o->name != n_) { continue; }
        return o;
    }
    return nullptr;
}

template <class T>  std::shared_ptr<Array<std::shared_ptr<T>>> GetTasks(String gn_) {
    std::shared_ptr<Array<std::shared_ptr<T>>> w = std::shared_ptr<Array<std::shared_ptr<T>>>(new Array<std::shared_ptr<T>>());
    /*for (auto& o : task) {
        if (o->groupName != gn_) { continue; }
        w->emplace_back(o);
    }*/
    return w;
}
