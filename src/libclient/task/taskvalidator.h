#ifndef TASKVALIDATOR_H
#define TASKVALIDATOR_H

#include "task.h"

class CLIENT_API TaskValidator
{
public:
    TaskValidator();
    ~TaskValidator();

    enum ValidationResult
    {
        Invalid,
        Valid // TODO: Find more texts
    };

    ValidationResult validate(const TestDefinitionPtr& testDefinition);
};

#endif // TASKVALIDATOR_H