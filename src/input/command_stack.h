#ifndef COMMAND_STACK_H
#define COMMAND_STACK_H

#include <vector>
#include <memory>

namespace input {

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
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
