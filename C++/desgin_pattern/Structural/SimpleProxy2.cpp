#include <iostream>
#include <memory>

// Subject interface
class Image {
public:
    virtual void display() = 0;
    virtual ~Image() = default;
};

// Real Subject
class RealImage : public Image {
private:
    std::string filename;

    void loadFromDisk() {
        std::cout << "Loading " << filename << " from disk.\n";
    }

public:
    RealImage(const std::string& file) : filename(file) {
        loadFromDisk();
    }

    void display() override {
        std::cout << "Displaying " << filename << "\n";
    }
};

// Proxy
class ProxyImage : public Image {
private:
    std::unique_ptr<RealImage> realImage;
    std::string filename;

public:
    ProxyImage(const std::string& file) : filename(file) {}

    void display() override {
        if (!realImage) {
            realImage = std::make_unique<RealImage>(filename);
        }
        realImage->display();
    }
};

int main() {
    Image* image1 = new ProxyImage("image1.jpg");
    Image* image2 = new ProxyImage("image2.jpg");

    // Image1 will be loaded and displayed
    image1->display();

    // Image1 will just be displayed, not loaded again
    image1->display();

    // Image2 will be loaded and displayed
    image2->display();

    delete image1;
    delete image2;

    return 0;
}