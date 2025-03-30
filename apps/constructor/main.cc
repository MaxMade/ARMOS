class T {
	int i = 0;
	public:
		T(int i) : i(i) { }

		operator int() const {
			return i;
		}
};

static T t(42);

int main(void) {
	return static_cast<int>(t);
}
