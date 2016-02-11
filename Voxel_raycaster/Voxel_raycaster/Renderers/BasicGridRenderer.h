#ifndef BASICGRIDRENDERER_H_
#define BASICGRIDRENDERER_H_


class Grid;
class RenderContext;

class BasicGridRenderer

{
public:
	BasicGridRenderer(void);
	void Render(RenderContext const& rc, Grid const* grid, unsigned char* texture_array) const;
	~BasicGridRenderer(void);
};


#endif /* BASICGRIDRENDERER_H_ */
