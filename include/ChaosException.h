#ifndef __CHAOS_EXCEPTION_H
#define __CHAOS_EXCEPTION_H

class ChaosException
{
public:
	explicit ChaosException(const std::string& msg) : m_msg(msg) {}

	std::string getMessage() const;

private:
	std::string m_msg;
};

#endif
