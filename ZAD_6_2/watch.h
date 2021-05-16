#ifndef WATCH_H
#define WATCH_H

enum TimeUnit {SECONDS,MINUTES};

struct WatchEvent{
	enum TimeUnit eTimeUnit;
	char TimeValue;
};

void Watch_Init(void);
struct WatchEvent sWatch_Read(void);
void Watch_Thread(void *pvParameters);




#endif
