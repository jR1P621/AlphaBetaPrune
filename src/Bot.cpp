class Bot
{
public:
    Bot(int (*utilityFunction)());

private:
    int (*utilityFunction)();
};

Bot::Bot(int (*utilityFunction)())
{
    this->utilityFunction = utilityFunction;
}