enum class PenType {
    GEL,
    BALL,
    FOUNTAIN,
    MARKER
};

#include <iostream>
using namespace std;

class WriteBehaviour {
public:
    virtual void write() = 0;
    virtual ~WriteBehaviour() {}
};

class SmoothWriteBehaviour : public WriteBehaviour {
public:
    void write() override {
        cout << "Writing smoothly..." << endl;
    }
};

class Ink {
private:
    string color;
public:
    Ink(string color) : color(color) {}
    string getColor() { return color; }
};

class Tip {
private:
    double size;
public:
    Tip(double size) : size(size) {}
    double getSize() { return size; }
};

class Refill {
private:
    Ink* ink;
    Tip* tip;

public:
    Refill(Ink* ink, Tip* tip) : ink(ink), tip(tip) {}

    Ink* getInk() { return ink; }
    Tip* getTip() { return tip; }
};

class Pen {
protected:
    PenType type;
    WriteBehaviour* writeBehaviour;

public:
    Pen(PenType type, WriteBehaviour* behaviour)
        : type(type), writeBehaviour(behaviour) {}

    virtual void write() {
        writeBehaviour->write();
    }

    PenType getType() {
        return type;
    }

    virtual ~Pen() {}
};

class GelPen : public Pen {
private:
    Refill* refill;
    bool canChangeRefill;

    // Private constructor
    GelPen(WriteBehaviour* behaviour)
        : Pen(PenType::GEL, behaviour),
          refill(nullptr),
          canChangeRefill(false) {}

public:
    class Builder {
    private:
        Refill* refill = nullptr;
        bool canChangeRefill = false;

    public:
        Builder& setRefill(Refill* r) {
            refill = r;
            return *this;
        }

        Builder& setCanChangeRefill(bool val) {
            canChangeRefill = val;
            return *this;
        }

        GelPen* build() {
            GelPen* pen = new GelPen(new SmoothWriteBehaviour());
            pen->refill = refill;
            pen->canChangeRefill = canChangeRefill;
            return pen;
        }
    };
};

class BallPen : public Pen {
public:
    BallPen() : Pen(PenType::BALL, new SmoothWriteBehaviour()) {}
};

class FountainPen : public Pen {
public:
    FountainPen() : Pen(PenType::FOUNTAIN, new SmoothWriteBehaviour()) {}
};

class Marker : public Pen {
public:
    Marker() : Pen(PenType::MARKER, new SmoothWriteBehaviour()) {}
};

class PenFactory {
public:
    static GelPen::Builder createGelPen() {
        return GelPen::Builder();
    }

    static BallPen* createBallPen() {
        return new BallPen();
    }

    static FountainPen* createFountainPen() {
        return new FountainPen();
    }

    static Marker* createMarker() {
        return new Marker();
    }
};

int main() {

    Ink* ink = new Ink("Blue");
    Tip* tip = new Tip(0.5);
    Refill* refill = new Refill(ink, tip);

    GelPen* gelPen = PenFactory::createGelPen()
                        .setCanChangeRefill(true)
                        .setRefill(refill)
                        .build();

    gelPen->write();

    BallPen* ballPen = PenFactory::createBallPen();
    ballPen->write();

    return 0;
}