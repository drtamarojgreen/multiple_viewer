#include "command_stack.h"

namespace input {

AddNodeCommand::AddNodeCommand(Graph& graph, const GraphNode& node)
    : graph_(graph), node_(node) {}

void AddNodeCommand::execute() {
    graph_.addNode(node_);
}

void AddNodeCommand::undo() {
    graph_.removeNode(node_.index);
}

RemoveNodeCommand::RemoveNodeCommand(Graph& graph, int nodeIndex)
    : graph_(graph), nodeIndex_(nodeIndex) {}

void RemoveNodeCommand::execute() {
    if (graph_.nodeExists(nodeIndex_)) {
        removedNode_ = graph_.nodeMap.at(nodeIndex_);
        graph_.removeNode(nodeIndex_);
    }
}

void RemoveNodeCommand::undo() {
    graph_.addNode(removedNode_);
}

void CommandStack::pushAndExecute(std::unique_ptr<ICommand> command) {
    command->execute();
    history_.push_back(std::move(command));
    redoStack_.clear();
}

void CommandStack::undo() {
    if (!history_.empty()) {
        auto cmd = std::move(history_.back());
        history_.pop_back();
        cmd->undo();
        redoStack_.push_back(std::move(cmd));
    }
}

void CommandStack::redo() {
    if (!redoStack_.empty()) {
        auto cmd = std::move(redoStack_.back());
        redoStack_.pop_back();
        cmd->execute();
        history_.push_back(std::move(cmd));
    }
}

} // namespace input
