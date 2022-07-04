//313CA - Bloțiu Mihnea-Andrei
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

//Structură pentru rotirea matricei color
typedef struct matrice {
	int red;
	int green;
	int blue;
} matrice;

void verificare_alocare_memorie(int **x)
{
	if (!x) {
		fprintf(stderr, "%s\n", "Eroare alocare memorie");
		exit(1);
	}
}

//încărcarea datelor separat din imaginile text sau binare
int **ld_img(char *fi, char word[], int *lat, int *ina, char ins[], int *OK)
{
	int **a;
	*OK = 0;
	FILE *in = fopen(fi, "rb");

	if (!in) {
		printf("%s", "Failed to load ");
		puts(fi);
		return NULL;
	}

	printf("%s ", "Loaded");
	puts(fi);
	*OK = 1;

	fgets(word, 3, in);
	fseek(in, 1, SEEK_CUR);
	fscanf(in, "%d %d", lat, ina);
	fseek(in, 1, SEEK_CUR);
	if (strcmp(word, "P2") == 0 || strcmp(word, "P5") == 0) {
		a = (int **)malloc(*ina * sizeof(int *));
		for (int i = 0; i < *ina; i++)
			a[i] = (int *)malloc(*lat * sizeof(int));
		verificare_alocare_memorie(a);
	} else {
		a = (int **)malloc((*ina) * sizeof(int *));
		for (int i = 0; i < *ina; i++)
			a[i] = (int *)malloc(3 * (*lat) * sizeof(int));
		verificare_alocare_memorie(a);
	}

	unsigned char c;
	if (strcmp(word, "P2") == 0) {
		fgets(ins, 4, in);
		fseek(in, 1, SEEK_CUR);
		for (int i = 0; i < *ina; i++)
			for (int j = 0; j < (*lat); j++)
				fscanf(in, "%d", &a[i][j]);
	}

	if (strcmp(word, "P3") == 0) {
		fgets(ins, 4, in);
		fseek(in, 1, SEEK_CUR);
		for (int i = 0; i < *ina; i++)
			for (int j = 0; j < 3 * (*lat); j++)
				fscanf(in, "%d", &a[i][j]);
		}

	if (strcmp(word, "P5") == 0) {
		fgets(ins, 4, in);
		fseek(in, 1, SEEK_CUR);
		for (int i = 0; i < *ina; i++)
			for (int j = 0; j < (*lat); j++) {
				fread((unsigned char *)&c, sizeof(unsigned char), 1, in);
				a[i][j] = (int)c;
			}
		}

	if (strcmp(word, "P6") == 0) {
		fgets(ins, 4, in);
		fseek(in, 1, SEEK_CUR);
		for (int i = 0; i < *ina; i++)
			for (int j = 0; j < 3 * (*lat); j++) {
				fread((unsigned char *)&c, sizeof(unsigned char), 1, in);
				a[i][j] = (int)c;
			}
		}

	fclose(in);
	return a;
}

//Interschimbare pentru ordinea elementelor din intervalul de selecție
void interschimbare(int *x, int *y)
{
	int aux;
	aux = *x;
	*x = *y;
	*y = aux;
}

//Selectarea regiunilor
int sel_reg(int *x1, int *y1, int *x2, int *y2, int lat, int ina, int OK)
{
	if (OK == 0) {
		printf("%s\n", "No image loaded");
		return 0;
	}

	if ((*x1) - (*x2) > 0)
		interschimbare(x1, x2);
	if ((*y1) - (*y2) > 0)
		interschimbare(y1, y2);
	if ((*x2) > lat || (*y2) > ina || (*x1) < 0 || (*y1) < 0) {
		printf("%s\n", "Invalid set of coordinates");
		return 0;
	}
	if ((*x2) <= 0 || (*y2) <= 0 || (*x1) >= lat || (*y1) >= ina) {
		printf("%s\n", "Invalid set of coordinates");
		return 0;
	}
	if ((*x1) == (*x2) || (*y1) == (*y2)) {
		printf("%s\n", "Invalid set of coordinates");
		return 0;
	}

	printf("%s", "Selected ");
	printf("%d %d %d %d\n", *x1, *y1, *x2, *y2);
	return 1;
}

//Selectarea întregii suprafețe
void select_all(int *x1, int *y1, int *x2, int *y2, int lat, int ina, int OK)
{
	if (OK == 0) {
		printf("%s\n", "No image loaded");
		return;
	}
	printf("%s\n", "Selected ALL");
	*x1 = 0;
	*x2 = lat;
	*y1 = 0;
	*y2 = ina;
}

//Rotirea efectiva a unui pătrat din matricea gayscale
void rotate90_square(int **a, int x1, int x2)
{
	int n = x2 - x1;
	for (int i = 0; i < n / 2; i++) {
		for (int j = i; j < n - i - 1; j++) {
			int temp = a[i][j];
			a[i][j] = a[n - 1 - j][i];
			a[n - 1 - j][i] = a[n - 1 - i][n - 1 - j];
			a[n - 1 - i][n - 1 - j] = a[j][n - 1 - i];
			a[j][n - 1 - i] = temp;
		}
	}
}

//Rotirea efectiva a unui pătrat din matricea color
void rotate90_square_color(matrice **a, int x1, int x2)
{
	int n = x2 - x1;
	for (int i = 0; i < n / 2; i++) {
		for (int j = i; j < n - i - 1; j++) {
			matrice temp = a[i][j];
			a[i][j] = a[n - 1 - j][i];
			a[n - 1 - j][i] = a[n - 1 - i][n - 1 - j];
			a[n - 1 - i][n - 1 - j] = a[j][n - 1 - i];
			a[j][n - 1 - i] = temp;
		}
	}
}

//Rotirea unui pătrat din matricea grayscale
//Copiem intr-o nouă matrice, o rotim, iar apoi revenim în matricea
//inițială în poziția corectă
void rot_sq_gray(int **a, int x1, int y1, int x2, int y2, int OK, int unghi)
{
	if (OK == 0) {
		printf("%s\n", "No image loaded");
		return;
	}
	int k = 0, l = 0;
	int **c = (int **)malloc((y2 - y1) * sizeof(int *));
	for (int i = 0; i < (y2 - y1); i++)
		c[i] = (int *)malloc((x2 - x1) * sizeof(int));

	for (int i = y1; i < y2; i++) {
		l = 0;
		for (int j = x1; j < x2; j++) {
			c[k][l] = a[i][j];
			l++;
		}
		k++;
	}

	if (unghi == 360 || unghi == -360 || unghi == 0) {
		printf("%s %d\n", "Rotated", unghi);
		return;
	} else if (unghi == 90 || unghi == -270) {
		rotate90_square(c, x1, x2);
	} else if (unghi == 180 || unghi == -180) {
		rotate90_square(c, x1, x2);
		rotate90_square(c, x1, x2);
	} else if (unghi == 270 || unghi == -90) {
		rotate90_square(c, x1, x2);
		rotate90_square(c, x1, x2);
		rotate90_square(c, x1, x2);
	} else {
		printf("%s\n", "Unsupported rotation angle");
		return;
	}

	printf("%s %d\n", "Rotated", unghi);

	for (int i = 0; i < k; i++)
		for (int j = 0; j < l; j++)
			a[y1 + i][x1 + j] = c[i][j];

	for (int i = 0; i < k; i++)
		free(c[i]);
	free(c);
}

//Rotirea unui pătrat din matricea color
//Copiem intr-o nouă matrice, o rotim, iar apoi revenim în matricea
//inițială în poziția corectă
void rot_sq_col(int **a, int x1, int y1, int x2, int y2, int OK, int unghi)
{
	if (OK == 0) {
		printf("%s\n", "No image loaded");
		return;
	}

	int k = 0, l = 0;
	matrice **rotire = (matrice **)malloc((y2 - y1) * sizeof(matrice *));
	for (int i = 0; i < (y2 - y1); i++)
		rotire[i] = (matrice *)malloc((x2 - x1) * sizeof(matrice));
	for (int i = y1; i < y2; i++) {
		l = 0;
		for (int j = 3 * x1; j < 3 * x2; j += 3) {
			rotire[k][l].red = a[i][j];
			rotire[k][l].green = a[i][j + 1];
			rotire[k][l].blue = a[i][j + 2];
			l++;
		}
		k++;
	}
	if (unghi == 360 || unghi == -360 || unghi == 0) {
		printf("%s %d\n", "Rotated", unghi);
		return;
	} else if (unghi == 90 || unghi == -270) {
		rotate90_square_color((matrice **)rotire, x1, x2);
	} else if (unghi == 180 || unghi == -180) {
		rotate90_square_color((matrice **)rotire, x1, x2);
		rotate90_square_color((matrice **)rotire, x1, x2);
	} else if (unghi == 270 || unghi == -90) {
		rotate90_square_color((matrice **)rotire, x1, x2);
		rotate90_square_color((matrice **)rotire, x1, x2);
		rotate90_square_color((matrice **)rotire, x1, x2);
	} else {
		printf("%s\n", "Unsupported rotation angle");
		return;
	}

	printf("%s %d\n", "Rotated", unghi);
	for (int i = 0; i < k; i++)
		for (int j = 0; j < l; j++) {
			a[y1 + i][3 * x1 + j * 3] = rotire[i][j].red;
			a[y1 + i][3 * x1 + j * 3 + 1] = rotire[i][j].green;
			a[y1 + i][3 * x1 + j * 3 + 2] = rotire[i][j].blue;
		}
	for (int i = 0; i < k; i++)
		free(rotire[i]);
	free(rotire);
}

//Rotirea efectiva a unei matrice grayscale
//Prin realocarea matricei inițiale cu dimensiuni inversate
int **rotate90_all_greyscale(int **a, int *inaltime, int *latime)
{
	int **b = (int **)malloc((*inaltime) * sizeof(int *));
	for (int i = 0; i < (*inaltime); i++) {
		b[i] = (int *)malloc((*latime) * sizeof(int));
		for (int j = 0; j < (*latime); j++)
			b[i][j] = a[i][j];
	}

	for (int i = 0; i < *inaltime; i++)
		free(a[i]);
	free(a);

	a = (int **)malloc((*latime) * sizeof(int *));
	for (int i = 0; i < *latime; i++) {
		a[i] = (int *)malloc((*inaltime) * sizeof(int));
		for (int j = 0; j < *inaltime; j++)
			a[i][j] = b[j][(*latime) - i - 1];
	}

	for (int i = 0; i < *inaltime; i++)
		free(b[i]);
	free(b);

	interschimbare(inaltime, latime);
	return a;
}

//Analog cu principiul de la rotirea unui pătrat
int  **rot_grey(int **a, int *ina, int *lat, int OK, int u, int *x2, int *y2)
{
	if (OK == 0) {
		printf("%s\n", "No image loaded");
		return a;
	}

	if (u == 0 || u == 360 || u == -360) {
		printf("%s %d\n", "Rotated", u);
		return a;
	} else if (u == -90 || u == 270) {
		a = rotate90_all_greyscale(a, ina, lat);
	} else if (u == 180 || u == -180) {
		a = rotate90_all_greyscale(a, ina, lat);
		a = rotate90_all_greyscale(a, ina, lat);
	} else if (u == 90 || u == -270) {
		a = rotate90_all_greyscale(a, ina, lat);
		a = rotate90_all_greyscale(a, ina, lat);
		a = rotate90_all_greyscale(a, ina, lat);
	} else {
		printf("%s\n", "Unsupported rotation angle");
		return a;
	}
	printf("%s %d\n", "Rotated", u);

	*x2 = *lat;
	*y2 = *ina;
	return a;
}

//Rotirea efectiva a unei matrice color
//Prin realocarea matricei inițiale cu dimensiuni inversate
int **rotate90_all_color(int **a, int *inaltime, int *latime)
{
	matrice **b = (matrice **)malloc((*inaltime) * sizeof(matrice *));
	for (int i = 0; i < *inaltime; i++) {
		b[i] = (matrice *)malloc((*latime) * sizeof(matrice));
		for (int j = 0; j < (*latime); j++) {
			b[i][j].red = a[i][3 * j];
			b[i][j].green = a[i][3 * j + 1];
			b[i][j].blue = a[i][3 * j + 2];
		}
	}

	for (int i = 0; i < *inaltime; i++)
		free(a[i]);
	free(a);

	a = (int **)malloc((*latime) * sizeof(int *));
	for (int i = 0; i < *latime; i++) {
		a[i] = (int *)malloc(3 * (*inaltime) * sizeof(int));
		for (int j = 0; j < (*inaltime); j++) {
			a[i][3 * j] = b[j][(*latime) - i - 1].red;
			a[i][3 * j + 1] = b[j][(*latime) - i - 1].green;
			a[i][3 * j + 2] = b[j][(*latime) - i - 1].blue;
		}
	}

	for (int i = 0; i < *inaltime; i++)
		free(b[i]);
	free(b);

	interschimbare(inaltime, latime);
	return a;
}

//Analog cu principiul de la rotirea unui pătrat
int  **rot_col(int **a, int *ina, int *lat, int OK, int u, int *x2, int *y2)
{
	if (OK == 0) {
		printf("%s\n", "No image loaded");
		return a;
	}

	if (u == 0 || u == 360 || u == -360) {
		printf("%s %d\n", "Rotated", u);
		return a;
	} else if (u == -90 || u == 270) {
		a = rotate90_all_color(a, ina, lat);
	} else if (u == 180 || u == -180) {
		a = rotate90_all_color(a, ina, lat);
		a = rotate90_all_color(a, ina, lat);
	} else if (u == 90 || u == -270) {
		a = rotate90_all_color(a, ina, lat);
		a = rotate90_all_color(a, ina, lat);
		a = rotate90_all_color(a, ina, lat);
	} else {
		printf("%s\n", "Unsupported rotation angle");
		return a;
	}

	printf("%s %d\n", "Rotated", u);

	*x2 = *lat;
	*y2 = *ina;
	return a;
}

//Operația de crop a unei imagini prin copiere într-o nouă matrice
//și schimbarea dimensiunilor conform selecției
int **r(int *x, int *y, int *z, int *t, int **a, char *w, int *m, int *n, int h)
{
	if (h == 0) {
		printf("%s\n", "No image loaded");
		return 0;
	}
	printf("%s\n", "Image cropped");
	int k = 0, l = 0;
	int **b;
	if (strcmp(w, "P2") == 0 || strcmp(w, "P5") == 0) {
		b = (int **)malloc(((*t) - (*y)) * sizeof(int *));
		for (int i = 0; i < ((*t) - (*y)); i++)
			b[i] = (int *)malloc(((*z) - (*x)) * sizeof(int));
		verificare_alocare_memorie(b);

		for (int i = *y; i < *t; i++) {
			l = 0;
			for (int j = *x; j < *z; j++) {
				b[k][l] = a[i][j];
				l++;
			}
			k++;
		}

		*m = l;

	} else {
		b = (int **)malloc(((*t) - (*y)) * sizeof(int *));
		for (int i = 0; i < ((*t) - (*y)); i++)
			b[i] = (int *)malloc(3 * ((*z) - (*x)) * sizeof(int));
		verificare_alocare_memorie(b);

		for (int i = *y; i < *t; i++) {
			l = 0;
			for (int j = 3 * (*x); j < 3 * (*z); j++) {
				b[k][l] = a[i][j];
				l++;
			}
			k++;
		}

		*m = l / 3;
	}

	for (int i = 0; i < *n; i++)
		free(a[i]);
	free(a);

	*n = k;
	*x = 0;
	*z = *m;
	*y = 0;
	*t = *n;
	return b;
}

//Rotunjirea necesară în operațille ce urmează
int rotunjire(double a)
{
	a = a * 10;
	int var = (int)a;
	if (var % 10 >= 5)
		return var / 10 + 1;
	else
		return var / 10;
}

int  **greyscale(int **a, int x1, int y1, int x2, int y2, int OK)
{
	if (OK == 0) {
		printf("%s\n", "No image loaded");
		return NULL;
	}
	printf("%s\n", "Grayscale filter applied");
	double t, s;
	for (int i = y1; i < y2; i++) {
		for (int j = 3 * x1; j < 3 * x2; j += 3) {
			t = a[i][j] + a[i][j + 1] + a[i][j + 2];
			s = t / 3;
			a[i][j] = rotunjire(s);
			a[i][j + 1] = rotunjire(s);
			a[i][j + 2] = rotunjire(s);
			t = 0;
		}
	}

	return a;
}

int  **sepia(int **a, int x1, int y1, int x2, int y2, int OK)
{
	double red, green, blue;
	if (OK == 0) {
		printf("%s\n", "No image loaded");
		return NULL;
	}
	printf("%s\n", "Sepia filter applied");

	for (int i = y1; i < y2; i++) {
		for (int j = 3 * x1; j < 3 * x2; j += 3) {
			red = 0.393 * a[i][j] + 0.769 * a[i][j + 1] + 0.189 * a[i][j + 2];
			green = 0.349 * a[i][j] + 0.686 * a[i][j + 1] + 0.168 * a[i][j + 2];
			blue = 0.272 * a[i][j] + 0.534 * a[i][j + 1] + 0.131 * a[i][j + 2];
			if ((int)red >= 255)
				a[i][j] = 255;
			else
				a[i][j] = rotunjire(red);

			if ((int)green >= 255)
				a[i][j + 1] = 255;
			else
				a[i][j + 1] = rotunjire(green);

			if ((int)blue >= 255)
				a[i][j + 2] = 255;
			else
				a[i][j + 2] = rotunjire(blue);
		}
	}

	return a;
}

//Funcția pentru salvarea unei poze în format binar în funcție de magic word
void s_i(char *fi, char word[], int *lat, int *ina, char ins[], int **a, int OK)
{
	if (OK == 0) {
		printf("%s\n", "No image loaded");
		return;
	}

	FILE *out = fopen(fi, "wb");

	if (!out) {
		printf("%s", "Failed to load ");
		puts(fi);
		return;
	}

	printf("%s ", "Saved");
	puts(fi);

	if (strcmp(word, "P2") == 0)
		strcpy(word, "P5");
	if (strcmp(word, "P3") == 0)
		strcpy(word, "P6");

	fprintf(out, "%s\n", word);

	fprintf(out, "%d %d\n", *lat, *ina);

	if (strcmp(word, "P5") == 0) {
		fprintf(out, "%s\n", ins);
		for (int i = 0; i < *ina; i++)
			for (int j = 0; j < (*lat); j++)
				fwrite(&a[i][j], sizeof(char), 1, out);
		}

	if (strcmp(word, "P6") == 0) {
		fprintf(out, "%s\n", ins);
		for (int i = 0; i < *ina; i++)
			for (int j = 0; j < 3 * (*lat); j++)
				fwrite(&a[i][j], sizeof(char), 1, out);
		}

	fclose(out);
}

//Funcția pentru salvarea unei poze în format text în funcție de magic_word
void s_ia(char *f, char word[], int *lat, int *ina, char ins[], int **a, int OK)
{
	if (OK == 0) {
		printf("%s\n", "No image loaded");
		return;
	}

	FILE *out = fopen(f, "wt");

	if (!out) {
		printf("%s", "Failed to load ");
		puts(f);
		return;
	}

	printf("%s ", "Saved");
	puts(f);

	if (strcmp(word, "P5") == 0)
		strcpy(word, "P2");
	if (strcmp(word, "P6") == 0)
		strcpy(word, "P3");

	fprintf(out, "%s\n", word);

	fprintf(out, "%d %d\n", *lat, *ina);

	if (strcmp(word, "P2") == 0) {
		fprintf(out, "%s\n", ins);
		for (int i = 0; i < *ina; i++) {
			for (int j = 0; j < (*lat); j++)
				fprintf(out, "%d ", a[i][j]);
			fprintf(out, "\n");
		}
	}

	if (strcmp(word, "P3") == 0) {
		fprintf(out, "%s\n", ins);
		for (int i = 0; i < *ina; i++) {
			for (int j = 0; j < 3 * (*lat); j++)
				fprintf(out, "%d ", a[i][j]);
			fprintf(out, "\n");
		}
	}

	fclose(out);
}

//În următoarele funcții identificăm tipul de rotire
//ce este solicitată pentru poza dorită
int ver_gray_sq(char word[], int x1, int y1, int x2, int y2)
{
	if (strcmp(word, "P2") == 0 || strcmp(word, "P5") == 0)
		if ((x2 - x1) == (y2 - y1))
			return 1;
	return 0;
}

int ver_col_sq(char word[], int x1, int y1, int x2, int y2)
{
	if (strcmp(word, "P3") == 0 || strcmp(word, "P6") == 0)
		if ((x2 - x1) == (y2 - y1))
			return 1;
	return 0;
}

int ver_int_gray(char word[], int x1, int y1, int x2, int y2, int lat, int ina)
{
	if (strcmp(word, "P2") == 0 || strcmp(word, "P5") == 0)
		if ((x2 - x1 == lat) && (y2 - y1) == ina)
			return 1;
	return 0;
}

int ver_int_col(char word[], int x1, int y1, int x2, int y2, int lat, int ina)
{
	if (strcmp(word, "P3") == 0 || strcmp(word, "P6") == 0)
		if ((x2 - x1 == lat) && (y2 - y1) == ina)
			return 1;
	return 0;
}

//Inițializarea coordonatelor imaginii
void inc_coord(int OK, int *x1, int *y1, int *x2, int *y2, int lat, int ina)
{
	if (OK == 1) {
		*x1 = 0;
		*y1 = 0;
		*x2 = lat;
		*y2 = ina;
	}
}

//Citirea datelor de la tastatură din cadrul operației de selectare
void citire(char *mat, int v[], int *nr, int *test)
{
	*nr = 0;
	*test = 0;
	while (mat) {
		for (int i = 0; i < (int)strlen(mat); i++) {
			if (isalpha(mat[i]))
				*test = 1;
		}
		v[*nr] = atoi(mat);
		(*nr)++;
		mat = strtok(NULL, " ");
	}
}

//Eliberarea memoriei unei matrice
void eliberare(int ina, int **a, int OK)
{
	if (OK == 1) {
		for (int i = 0; i < ina; i++)
			free(a[i]);
		free(a);
	} else {
		printf("%s\n", "No image loaded");
	}
}

//Actualizarea coordonatelor imaginii
void ac_crd(int v[], int *x1, int *y1, int *x2, int *y2)
{
	*x1 = v[0];
	*y1 = v[1];
	*x2 = v[2];
	*y2 = v[3];
}

int main(void)
{
	char comanda[10], word[3], ins[4], nume_fisier[100], continuare[30];
	int **a, **b, lat, ina, x1 = 0, x2, y1 = 0, y2, OK = 0, v[4], unghi;
	int nr = 0, test = 0;
	char nume_fisier_salvare[100], parametru[10] = "check", rest[10];
	scanf("%s", comanda);
	//Verificarea tipului comenzii primite și executarea instrucțiunilor
	while (strcmp(comanda, "EXIT") != 0) {
		if (strcmp(comanda, "LOAD") == 0) {
			scanf("%s", nume_fisier);
			a = ld_img(nume_fisier, word, &lat, &ina, ins, &OK);
			inc_coord(OK, &x1, &y1, &x2, &y2, lat, ina);
			scanf("%s", comanda);
		} else if (strcmp(comanda, "SELECT") == 0) {
			fgets(continuare, 30, stdin);
			char *mat = strtok(continuare, " ");
			if (strcmp(mat, "ALL") == 0 || strcmp(mat, "ALL\n") == 0) {
				select_all(&x1, &y1, &x2, &y2, lat, ina, OK);
				scanf("%s", comanda);
			} else {
				citire(mat, v, &nr, &test);
				if (nr == 4 && test == 0) {
					if (sel_reg(&v[0], &v[1], &v[2], &v[3], lat, ina, OK))
						ac_crd(v, &x1, &y1, &x2, &y2);
				} else {
					printf("%s\n", "Invalid command");
				}
				scanf("%s", comanda);
			}
		} else if (strcmp(comanda, "ROTATE") == 0) {
			scanf("%d", &unghi);
			if (OK == 0) {
				printf("%s\n", "No image loaded");
				scanf("%s", comanda);
			} else if (ver_gray_sq(word, x1, y1, x2, y2)) {
				rot_sq_gray(a, x1, y1, x2, y2, OK, unghi);
			} else if (ver_col_sq(word, x1, y1, x2, y2)) {
				rot_sq_col(a, x1, y1, x2, y2, OK, unghi);
			} else if (ver_int_gray(word, x1, y1, x2, y2, lat, ina)) {
				a = rot_grey(a, &ina, &lat, OK, unghi, &x2, &y2);
			} else if (ver_int_col(word, x1, y1, x2, y2, lat, ina)) {
				a = rot_col(a, &ina, &lat, OK, unghi, &x2, &y2);
			}
			if (OK == 1)
				scanf("%s", comanda);
		} else if (strcmp(comanda, "CROP") == 0) {
			b = r(&x1, &y1, &x2, &y2, a, word, &lat, &ina, OK);
			a = b;
			scanf("%s", comanda);
		} else if (strcmp(comanda, "GRAYSCALE") == 0) {
			if (strcmp(word, "P2") == 0 || strcmp(word, "P5") == 0)
				printf("%s\n", "Grayscale filter not available");
			else
				a = greyscale(a, x1, y1, x2, y2, OK);
			scanf("%s", comanda);
		} else if (strcmp(comanda, "SEPIA") == 0) {
			if (strcmp(word, "P2") == 0 || strcmp(word, "P5") == 0) {
				printf("%s\n", "Sepia filter not available");
				scanf("%s", comanda);
			} else {
				a = sepia(a, x1, y1, x2, y2, OK);
				scanf("%s", comanda);
			}
		} else if (strcmp(comanda, "SAVE") == 0) {
			scanf("%s %s", nume_fisier_salvare, parametru);
			if (strcmp(parametru, "ascii") == 0) {
				s_ia(nume_fisier_salvare, word, &lat, &ina, ins, a, OK);
				scanf("%s", comanda);
			} else {
				s_i(nume_fisier_salvare, word, &lat, &ina, ins, a, OK);
				strcpy(comanda, parametru);
			}
		} else if (strcmp(comanda, "EXIT") != 0) {
			printf("%s\n", "Invalid command");
			fgets(rest, 10, stdin);
			scanf("%s", comanda);
		}
	}
	eliberare(ina, a, OK);
	return 0;
}
