#ifndef COMMAND_STACK_H
#define COMMAND_STACK_H

#include <vector>
#include <memory>
#include "map_logic.h"

namespace input {

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class AddNodeCommand : public ICommand {
public:
    AddNodeCommand(Graph& graph, const GraphNode& node);
    void execute() override;
    void undo() override;

private:
    Graph& graph_;
    GraphNode node_;
};

class RemoveNodeCommand : public ICommand {
public:
    RemoveNodeCommand(Graph& graph, int nodeIndex);
    void execute() override;
    void undo() override;

private:
    Graph& graph_;
    int nodeIndex_;
    GraphNode removedNode_;
};

class CommandStack {
public:
    void pushAndExecute(std::unique_ptr<ICommand> command);
    void undo();
    void redo();

private:
    std::vector<std::unique_ptr<ICommand>> history_;
    std::vector<std::unique_ptr<ICommand>> redoStack_;
};

} // namespace input

#endif // COMMAND_STACK_H
