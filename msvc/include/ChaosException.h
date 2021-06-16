#pragma once

class ChaosException
{
public:
    explicit ChaosException(const std::string& msg)
      : m_msg(msg)
    {
    }

    std::string getMessage() const;

private:
    std::string m_msg;
};
