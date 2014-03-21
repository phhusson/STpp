#ifndef _LIDAR_H
#define _LIDAR_H

#include <IStream.h>
typedef struct {
	char index;
	uint16_t speed;
	uint32_t data[4];
	uint16_t checksum;
} lidar_neato_t;

IStream& operator>>(IStream& i, lidar_neato_t& p);

class LidarNeato {
	public:
		LidarNeato(Gpio& rx, Uart& u);
};

#endif /* _LIDAR_H */
