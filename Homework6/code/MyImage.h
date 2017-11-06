#pragma once

#include <cassert>
#include <vector>
#include <string>
#include <sys/stat.h>


class MyImage
{
private:
	std::vector<BYTE> pixels;
    int w, h, comp;

public:
    MyImage():w(0),h(0),comp(0) {}
    ~MyImage() { }

	MyImage(const std::string &filename, int ncomp=4):w(0), h(0),comp(0)
	{
        // not implemented
    }


    MyImage(BYTE* data, int ww, int hh, int pitch, int ncomp = 3) :w(ww), h(hh), comp(ncomp)
    {
        assert(pitch >= ww * 3);
		if (pitch == w*comp) pixels = std::vector<BYTE>(data, data + pitch*h);
		else {
			pixels.resize(w*comp*h);
			for (int i = 0; i < h; i++) std::copy_n(data + pitch*i, pitch, pixels.data() + i*w*comp);
		}
	}

    static int alignment() { return 1; }  // OpenGL only supports 1,2,4,8, do not use 8, it is buggy

    inline bool empty() const { return pixels.empty(); }

	inline BYTE* data() { return pixels.data(); }
	inline const BYTE* data() const { return pixels.data(); }
	inline int width() const { return w; }
	inline int height() const { return h; }
	inline int dim() const { return comp; }
	inline int pitch() const { return w*comp; }


    MyImage resizeCanvas(int ww, int hh)
    {
		std::vector<BYTE> data(ww*comp*hh, 255);
		for (int i = 0; i < h; i++)
			std::copy_n(pixels.data() + i*w*comp, w*comp, data.data() + i*ww*comp);
 
		return MyImage(data.data(), ww, hh, ww*comp, comp);   
	}


    inline void write(const std::string &filename, bool vflip=true) const {
        // not implemented
	}

	inline std::vector<BYTE> bits(int align=1) const
	{
        const int pitch = (w * comp + align - 1) / align*align;

		std::vector<BYTE> data(pitch*h);
		for(int i=0; i<h; i++)
			std::copy_n(pixels.data()+i*w*comp, w*comp, data.data()+i*pitch);

		return data;
	}
};

