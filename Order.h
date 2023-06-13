#ifndef ORDER_H
#define ORDER_H

#include <string>

class TaskExecuteSystem;

class Order
{
public:
    virtual ~Order() {};

    virtual void executeOrder(TaskExecuteSystem &taskExecuteSystem, int tileIndex) = 0;

    std::string getOrderText();

protected:
    std::string orderText;
};

/* class OrderCutTree inheriting from Order */
class OrderCutTree : public Order
{
public:
    OrderCutTree();

    void executeOrder(TaskExecuteSystem &taskExecuteSystem, int tileIndex) override;
};

/* class OrderBuildWall inheriting from Order */
class OrderBuildWall : public Order
{
public:
	OrderBuildWall();

	void executeOrder(TaskExecuteSystem & taskExecuteSystem, int tileIndex) override;
};

/* class OrdrCancel inheriting from Order */
class OrderCancel : public Order
{
public:
    OrderCancel();

    void executeOrder(TaskExecuteSystem &taskExecuteSystem, int tileIndex) override;
};

#endif // ORDER_H
