class Transformation implements Cloneable {
	public:  Matrix matrix;
		 Matrix inverse;

	/**
	* Reference to the identity {@link Transformation}.
	*/
	public static final Transformation IDENTITY = new Transformation(
		Matrix.IDENTITY, Matrix.IDENTITY);

	/**
	* Creates a new {@link Transformation} for three dimensional objects.
	*
	* @param matrix
	*            the matrix transformation.
	* @param inverse
	*            the inverse of the given transformation.
	*/
	private Transformation(Matrix matrix, Matrix inverse) {
		this.matrix = matrix;
		this.inverse = inverse;
	}

	/**
	* Creates a copy of the given {@link Transformation}.
	*
	* @param transformation
	*            the {@link Transformation} to copy.
	* @throws NullPointerException
	*             when the given {@link Transformation} is null.
	*/
	public Transformation(Transformation transformation)
		throws NullPointerException {
		this.matrix = transformation.matrix;
		this.inverse = transformation.inverse;
	}

	/**
	* Returns the {@link Matrix} containing the transformation.
	*
	* @return the {@link Matrix} containing the transformation.
	*/
	public Matrix getTransformationMatrix() {
		return matrix;
	}

	/**
	* Returns the inverse of the {@link Matrix} containing the transformation.
	*
	* @return the inverse of the {@link Matrix} containing the transformation.
	*/
	public Matrix getInverseTransformationMatrix() {
		return inverse;
	}

	/**
	* Returns the inverse of this {@link Transformation}.
	*
	* @return the inverse of this {@link Transformation}.
	*/
	public Transformation invert() {
		return new Transformation(inverse, matrix);
	}

	/**
	* Appends the given {@link Transformation} to this {@link Transformation}.
	*
	* @param transformation
	*            the {@link Transformation} to append.
	* @throws NullPointerException
	*             when the given {@link Transformation} is null.
	* @return this {@link Transformation} concatenated with the given
	*         {@link Transformation}.
	*/
	public Transformation append(Transformation transformation)
		throws NullPointerException {
		return new Transformation(matrix.multiply(transformation.matrix),
			transformation.inverse.multiply(inverse));
	}

	/**
	* Appends the given {@link Transformation} to the right of this {@link Transformation}.
	* That is, the given {@link Transformation} will be applied  before this {@link Transformation}..
	*
	* @param transformation
	*             the {@link Transformation} to append to the right.
	* @return this {@link Transformation} concatenated to the left of the given
	*         {@link Transformation}.
	* @throws NullPointerException
	*              when the given {@link Transformation} is null.
	*/
	public Transformation appendToTheRight(Transformation transformation)
		throws NullPointerException {
		return new Transformation(matrix.multiply(transformation.matrix),
			transformation.inverse.multiply(inverse));
	}

	/**
	* Appends the given {@link Transformation} to the left of this {@link Transformation}.
	* That is, the given {@link Transformation} will be applied after this {@link Transformation}..
	*
	* @param transformation
	*             the {@link Transformation} to append to the left.
	* @return this {@link Transformation} concatenated to the right of the given
	*         {@link Transformation}.
	* @throws NullPointerException
	*              when the given {@link Transformation} is null.
	*/
	public Transformation appendToTheLeft(Transformation transformation)
		throws NullPointerException {
		return new Transformation(transformation.matrix.multiply(matrix),
			inverse.multiply(transformation.inverse));
	}

	/**
	* Transforms the given {@link Point} with this {@link Transformation}.
	*
	* @param point
	*            the {@link Point} to transform.
	* @throws NullPointerException
	*             when the given {@link Point} is null.
	* @return the given {@link Point} transformed by this
	*         {@link Transformation}.
	*/
	public Point transform(Point point) throws NullPointerException {
		return matrix.transform(point);
	}

	/**
	* Transforms the given {@link Point} with the inverse of this
	* {@link Transformation}.
	*
	* @param point
	*            the {@link Point} to transform.
	* @throws NullPointerException
	*             when the given {@link Point} is null.
	* @return the given {@link Point} transformed by the inverse of this
	*         {@link Transformation}.
	*/
	public Point transformInverse(Point point) throws NullPointerException {
		return inverse.transform(point);
	}

	/**
	* Transforms the given {@link Vector} with this {@link Transformation}.
	*
	* @param vector
	*            the {@link Vector} to transform.
	* @throws NullPointerException
	*             when the given {@link Vector} is null.
	* @return the given {@link Vector} transformed by this
	*         {@link Transformation}.
	*/
	public Vector transform(Vector vector) throws NullPointerException {
		return matrix.transform(vector);
	}

	/**
	* Transforms the given {@link Vector} with the inverse of this
	* {@link Transformation}.
	*
	* @param vector
	*            the {@link Vector} to transform.
	* @throws NullPointerException
	*             when the given {@link Vector} is null.
	* @return the given {@link Vector} transformed by the inverse of this
	*         {@link Transformation}.
	*/
	public Vector transformInverse(Vector vector) throws NullPointerException {
		return inverse.transform(vector);
	}

	/**
	* Transforms the given {@link Ray} with this {@link Transformation}.
	*
	* @param ray
	*            the {@link Ray} to transform.
	* @throws NullPointerException
	*             when the given {@link Ray} is null.
	* @return the given {@link Ray} transformed by this {@link Transformation}.
	*/
	public Ray transform(Ray ray) throws NullPointerException {
		Point point = transform(ray.origin);
		Vector direction = transform(ray.direction);
		return new Ray(point, direction);
	}

	/**
	* Transforms the given {@link Ray} with the inverse of this
	* {@link Transformation}.
	*
	* @param ray
	*            the {@link Ray} to transform.
	* @throws NullPointerException
	*             when the given {@link Ray} is null.
	* @return the given {@link Ray} transformed by the inverse of this
	*         {@link Transformation}.
	*/
	public Ray transformInverse(Ray ray) throws NullPointerException {
		Point point = transformInverse(ray.origin);
		Vector direction = transformInverse(ray.direction);
		return new Ray(point, direction);
	}

	/**
	* Returns the identity {@link Transformation}.
	*
	* @return the identity {@link Transformation}.
	*/
	public static Transformation createIdentity() {
		return IDENTITY;
	}

	/**
	* Creates a new translation {@link Transformation}.
	*
	* @param x
	*            the x translation for this {@link Transformation}.
	* @param y
	*            the y translation for this {@link Transformation}.
	* @param z
	*            the z translation for this {@link Transformation}.
	* @return a new translation {@link Transformation}.
	*/
	public static Transformation createTranslation(double x, double y, double z) {
		// @formatter:off
		Matrix transformation = new Matrix(1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1);
		Matrix inverse = new Matrix(1, 0, 0, -x,
			0, 1, 0, -y,
			0, 0, 1, -z,
			0, 0, 0, 1);
		// @formatter:on
		return new Transformation(transformation, inverse);
	}

	/**
	* Creates a new scale {@link Transformation}.
	*
	* @param x
	*            the x scale for this scale {@link Transformation}.
	* @param y
	*            the y scale for this scale {@link Transformation}.
	* @param z
	*            the z scale for this scale {@link Transformation}.
	* @return a new {@link Transformation} which scales three dimensional
	*         objects.
	*/
	public static Transformation createScale(double x, double y, double z) {
		// @formatter:off
		Matrix transformation = new Matrix(x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
		Matrix inverse = new Matrix(1 / x, 0, 0, 0,
			0, 1 / y, 0, 0,
			0, 0, 1 / z, 0,
			0, 0, 0, 1);
		// @formatter:on
		return new Transformation(transformation, inverse);
	}

	/**
	* Creates a new rotation {@link Transformation} about the x axis in a
	* counter clockwise direction.
	*
	* @param angle
	*            the angle to rotate about (in degrees).
	* @return a new rotation {@link Transformation} about the x axis.
	*/
	public static Transformation createRotationX(double angle) {
		double rad = Math.toRadians(angle);
		double sin = Math.sin(rad);
		double cos = Math.cos(rad);

		// @formatter:off
		Matrix transformation = new Matrix(1, 0, 0, 0,
			0, cos, -sin, 0,
			0, sin, cos, 0,
			0, 0, 0, 1);
		Matrix inverse = transformation.transpose();
		// @formatter:on
		return new Transformation(transformation, inverse);
	}

	/**
	* Creates a new rotation {@link Transformation} about the y axis in a
	* counter clockwise direction.
	*
	* @param angle
	*            the angle to rotate about (in degrees).
	* @return a new rotation {@link Transformation} about the y axis.
	*/
	public static Transformation createRotationY(double angle) {
		double rad = Math.toRadians(angle);
		double sin = Math.sin(rad);
		double cos = Math.cos(rad);

		// @formatter:off
		Matrix transformation = new Matrix(cos, 0, sin, 0,
			0, 1, 0, 0,
			-sin, 0, cos, 0,
			0, 0, 0, 1);
		Matrix inverse = transformation.transpose();
		// @formatter:on
		return new Transformation(transformation, inverse);
	}

	/**
	* Creates a new rotation {@link Transformation} about the z axis in a
	* counter clockwise direction.
	*
	* @param angle
	*            the angle to rotate about (in degrees).
	* @return a new rotation {@link Transformation} about the z axis.
	*/
	public static Transformation createRotationZ(double angle) {
		double rad = Math.toRadians(angle);
		double sin = Math.sin(rad);
		double cos = Math.cos(rad);

		// @formatter:off
		Matrix transformation = new Matrix(cos, -sin, 0, 0,
			sin, cos, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		Matrix inverse = transformation.transpose();
		// @formatter:on
		return new Transformation(transformation, inverse);
	}


}

