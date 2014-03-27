#ifndef _LIDAR_H
#define _LIDAR_H

#include <IStream.h>
#include <Lock.h>
typedef struct {
	char index;
	uint16_t speed;
	uint32_t data[4];
	uint16_t checksum;
} lidar_neato_t;

class LidarNeato;
LidarNeato& operator>>(LidarNeato& i, lidar_neato_t& p);

class LidarNeato {
	private:
		uint16_t distances[360];
		Mutex distancesLock;
		Uart &uart;
	public:
		uint16_t getDistance(int pos);
		LidarNeato(Gpio& rx, Uart& u);
		friend LidarNeato& operator>>(LidarNeato& i, lidar_neato_t& p);
};

#endif /* _LIDAR_H */
