#pragma once
class CTank
{
public:
	CTank(void);
	~CTank(void);

	unsigned char m_x; 
	unsigned char m_y; 

	void MoveTo( unsigned char x, unsigned char y ) {
		this->m_x = x; this->m_y = y ; 
	}
};

