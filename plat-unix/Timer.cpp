#include <Timer.h>
#include <Log.h>

Timer::Timer(volatile TIM_TypeDef* b, int n) :
	base(b), number(n) {
}

int Timer::getNumber() {
	return number;
}

unsigned short Timer::getAutoReload() {
	return 0;
}

Timer& Timer::setAutoReload(unsigned short v) {
	return *this;
}

Timer& Timer::setPrescaler(unsigned short v) {
	return *this;
}

Timer& Timer::setAutoReloadBuffered(bool v) {
	return *this;
}

Timer& Timer::setCounter(unsigned short v) {
	return *this;
}

unsigned short Timer::getCounter() {
	return 0;
}

Timer& Timer::enable() {
	return *this;
}

bool Timer::enabled() {
	return false;
}

Timer& Timer::disable() {
	return *this;
}

Timer& Timer::setChannelDirection(int chan, Direction d) {
	return *this;
}

Timer& Timer::setChannelMode(int chan, ChannelMode m) {
	return *this;
}

Timer& Timer::setChannelOutput(int chan, bool o) {
	return *this;
}

Timer& Timer::setChannelComparator(int chan, unsigned short v) {
	return *this;
}

Timer& Timer::wait() {
	return *this;
}

Timer& Timer::setOneShot(bool v) {
	return *this;
}

Timer::operator bool() {
	return false;
}

Timer& Timer::update() {
	return *this;
}

Timer& Timer::setCaptureCompare1Sel(int v){
	return *this;
}

Timer& Timer::setCaptureCompare2Sel(int v){
	return *this;
}

Timer& Timer::setCaptureCompare1Polarity(bool in, int v){
	return *this;
}

Timer& Timer::setCaptureCompare2Polarity(bool in, int v){
	return *this;
}

Timer& Timer::setInputCapture1Filter(int v){
	return *this;
}

Timer& Timer::setInputCapture2Filter(int v){
	return *this;
}

Timer& Timer::setSlaveModeSelection(int v){
	return *this;
}


//Interrupt handling
Timer& Timer::setUIE(bool s) {
	return *this;
}

Timer& Timer::setURS(bool onlyOverflow) {
	return *this;
}

Timer& Timer::clearInterrupt() {
	return *this;
}

static Timer::Callback topCBs[15];
void Timer::callTopCb(int nr) {
	topCBs[nr](nr);
}

Timer& Timer::setTopCB(Callback cb) {
	topCBs[number] = cb;
	return *this;
}

int Timer::irqNr() {
	return 0;
}
