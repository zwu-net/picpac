#include <arpa/inet.h>
#include <boost/filesystem/fstream.hpp>
#include "picpac.h"
#include "picpac-cv.h"
#include "picpac-util.h"

using namespace std;
using namespace boost;
using namespace picpac;

void load (fs::path const &images_path,
           fs::path const &labels_path,
           fs::path const &output_path) {
    ImageEncoder encoder(".png");
    picpac::FileWriter write(output_path, FileWriter::COMPACT);
    fs::ifstream images(images_path, ios::binary);
    fs::ifstream labels(labels_path, ios::binary);
    CHECK(images);
    CHECK(labels);
    int32_t magic;
    images.read((char *)&magic, sizeof(magic));
    magic = ntohl(magic);
    CHECK(magic == 0x00000803);
    labels.read((char *)&magic, sizeof(magic));
    magic = ntohl(magic);
    CHECK(magic == 0x00000801);
    int32_t n, n1;
    images.read((char *)&n, sizeof(n));
    n = ntohl(n);
    labels.read((char *)&n1, sizeof(n1));
    n1 = ntohl(n1);
    CHECK(n == n1);
    int32_t rows, cols;
    images.read((char *)&rows, sizeof(rows));
    rows = ntohl(rows);
    images.read((char *)&cols, sizeof(cols));
    cols = ntohl(cols);
    CHECK(rows == 28);
    CHECK(cols == 28);
    cv::Mat image(rows, cols, CV_8UC1);
    char label;
    string buf;
    for (int i = 0; i < n; ++i) {
        images.read(image.ptr<char>(0), image.total());
        CHECK(images);
        labels.read(&label, sizeof(label));
        CHECK(labels);
        encoder.encode(image, &buf);
        picpac::Record rec((int)label, buf);
        write.append(rec);
    }
}

int main(int argc, char **argv) {
    load("train-images-idx3-ubyte", "train-labels-idx1-ubyte", "train.db");
    load("t10k-images-idx3-ubyte", "t10k-labels-idx1-ubyte", "test.db");
    return 0;
}


