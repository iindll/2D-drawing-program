#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <sstream>
#include <list>
#define MAXENTRIES 600
using namespace std;



/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);
HMENU hmenu;
HMENU bmenu;
HWND g_hWnd;
/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain(HINSTANCE hThisInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpszArgument,
    int nCmdShow)
{
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);  // <-- ADD THIS

    HWND hwnd;
    /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_HAND);/**mouse look*/
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;
    /* structure or the window instance */
    /* Use Windows's White colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;/**window color*/
    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
        0,                   /* Extended possibilites for variation */
        szClassName,         /* Classname */
        _T("Code::Blocks Template Windows App"),       /* Title Text */
        WS_OVERLAPPEDWINDOW, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        900,                 /* The programs width */
        600,                 /* and height in pixels */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        NULL,                /* No menu */
        hThisInstance,       /* Program Instance handler */
        NULL/* No Window Creation data */
    );

    /* Make the window visible on the screen */
    //Use it in clean screen
    ShowWindow(hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/**read file*/
void read(vector<int>& arr, string points)
{
    stringstream ss(points); /*Read the points from the file*/

    for (int i; ss >> i;)  /*read the points and store it in vector and ignore the ',' char*/
    {
        arr.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }
}

void prev(HDC hdc, vector<int> numbers, COLORREF c)
{
    for (int i = 0; i < numbers.size(); i += 2)   /*every two points we set the pixel that has been stored in the sort vector*/
    {
        SetPixel(hdc, numbers[i], numbers[i + 1], c);  /*i=0,i=1 setpixel(0,1) // i=2,i=3 setpixel(2,3) and so on     */
    }
}

void swap(int& x, int& y) {
    int tmp = x;
    x = y;
    y = tmp;
}

//int xc = 0, yc = 0, R = 0; // Initialize to 0

boolean insideCircle(int x, int y, int xc, int yc, int r) {
    int d = sqrt(pow((x - xc), 2) + pow((y - yc), 2));
    return (d <= r);

}
/**to save point */
vector<int> savedPoints;
void save(int x, int y)
{
    savedPoints.push_back(x);
    savedPoints.push_back(y);
}
/**save files*/
void SaveInFile()
{
    ofstream file;
    file.open("save.txt");   /*open the file and start to storing in it */
    for (int i = 0; i < savedPoints.size(); i += 2)
        file << savedPoints[i] << "," << savedPoints[i + 1] << ",";

    file.close();
}
/**to draw 8 points*/
void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc - x, yc - y, c);
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc + y, yc + x, c);
    SetPixel(hdc, xc - y, yc + x, c);
    SetPixel(hdc, xc - y, yc - x, c);
    SetPixel(hdc, xc + y, yc - x, c);

    save(xc + x, yc + y);
    save(xc - x, yc + y);
    save(xc - x, yc - y);
    save(xc + x, yc - y);
    save(xc + y, yc + x);
    save(xc - y, yc + x);
    save(xc - y, yc - x);
    save(xc + y, yc - x);


}

int Round(double x)
{
    return (int)(0.5 + x);
}
struct Vector {
    double v[2];
    Vector(double x = 0, double y = 0)
    {
        v[0] = x; v[1] = y;
    }
    double& operator[](int i) {
        return v[i];
    }
};
struct Vector2
{
    double x, y;
    Vector2(double a = 0, double b = 0)
    {
        x = a; y = b;
    }

};
/**lines codes*/
void parametric(HDC hdc, int x1, int y1, int x2, int y2, COLORREF C)
{

    int dx = x2 - x1;
    int dy = y2 - y1;
    double dt = 1.0 / max(dx, dy);
    for (double t = 0; t < 1; t += dt)
    {
        double x = x1 + t * dx;
        double y = y1 + t * dy;
        SetPixel(hdc, Round(x), Round(y), C);
        save(Round(x), Round(y));
    }
}

void DDALine(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color)
{
    int dx = xe - xs;
    int dy = ye - ys;
    SetPixel(hdc, xs, ys, color);
    save(xs, ys);
    if (abs(dx) >= abs(dy))
    {
        int x = xs, xinc = dx > 0 ? 1 : -1;
        double y = ys, yinc = (double)dy / dx * xinc;
        while (x != xe)
        {
            x += xinc;
            y += yinc;
            SetPixel(hdc, x, round(y), color);
            save(x, y);
        }
    }
    else
    {
        int y = ys, yinc = dy > 0 ? 1 : -1;
        double x = xs, xinc = (double)dx / dy * yinc;
        while (y != ye)
        {
            x += xinc;
            y += yinc;
            SetPixel(hdc, round(x), y, color);
            save(x, y);
        }
    }
}

void midpoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF C)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int x = x1;
    int y = y1;

    int sx = (x2 >= x1) ? 1 : -1; // step x
    int sy = (y2 >= y1) ? 1 : -1; // step y

    SetPixel(hdc, x, y, C);
    save(x, y);

    if (dx > dy)
    {
        int d = 2 * dy - dx;
        int d1 = 2 * dy;
        int d2 = 2 * (dy - dx);

        while (x != x2)
        {
            if (d <= 0)
            {
                d += d1;
            }
            else
            {
                d += d2;
                y += sy;
            }
            x += sx;
            SetPixel(hdc, x, y, C);
            save(x, y);
        }
    }
    else
    {
        int d = 2 * dx - dy;
        int d1 = 2 * dx;
        int d2 = 2 * (dx - dy);

        while (y != y2)
        {
            if (d <= 0)
            {
                d += d1;
            }
            else
            {
                d += d2;
                x += sx;
            }
            y += sy;
            SetPixel(hdc, x, y, C);
            save(x, y);
        }
    }
}



/**circle codes */
void DirectPolar(HDC hdc, int xc, int yc, int R, COLORREF C) {
    int x = R, y = 0;
    double theta = 0, dtheta = 1.0 / R;
    Draw8Points(hdc, xc, yc, x, y, C);
    while (x > y)
    {
        theta += dtheta;
        x = round(R * cos(theta));
        y = round(R * sin(theta));
        Draw8Points(hdc, xc, yc, x, y, C);
    }

}

void Directcircle(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0, y = R;
    int R2 = R * R;
    Draw8Points(hdc, xc, yc, x, y, c);
    while (x < y)
    {
        x++;
        y = round(sqrt((double)(R2 - x * x)));
        Draw8Points(hdc, xc, yc, x, y, c);
    }
}
void itreativepolar(HDC hdc, int xc, int yc, int R, COLORREF C) {

    double x = R, y = 0;
    double dtheta = 1.0 / R;
    double c = cos(dtheta), s = sin(dtheta);
    Draw8Points(hdc, xc, yc, round(x), round(y), C);
    while (x > y)
    {
        double x1 = x * c - y * s;
        y = x * s + y * c;
        x = x1;
        Draw8Points(hdc, xc, yc, round(x), round(y), C);
    }
}
void midpointCircle(HDC hdc, int xc, int yc, int r, COLORREF C) {
    int x = 0;
    int y = r;
    double d = 1 - r;

    Draw8Points(hdc, xc, yc, x, y, C);

    while (x < y)
    {
        if (d <= 0) {
            d = d + 2 * x + 3;
            x++;
        }
        else {
            d = d + 2 * (x - y) + 5;
            x++;
            y--;
        }

        Draw8Points(hdc, xc, yc, x, y, C);
    }
}
void modifiedMidpoint(HDC hdc, int xc, int yc, int r, COLORREF C) {
    int x = 0, y = r;
    int d = 1 - r;
    int d1 = 3;
    int d2 = 5 - 2 * r;
    while (x <= y)
    {
        if (d < 0)
        {
            d += d1;
            d2 += 2;
            d1 += 2;
            x++;
        }
        else
        {
            d += d2;
            d2 += 4;
            y--;
            d1 += 2;
            x++;
        }
        Draw8Points(hdc, xc, yc, Round(x), Round(y), C);
    }
}


/**flood fill*/
void RFloodFill(HDC hdc, int x, int y, COLORREF Cb, COLORREF Cf, int n)
{
    COLORREF C = GetPixel(hdc, x, y);
    if (C == Cb || C == Cf)return;
    SetPixel(hdc, x, y, Cf);
    RFloodFill(hdc, x + 1, y, Cb, Cf, 6);
    RFloodFill(hdc, x - 1, y, Cb, Cf, 6);
    RFloodFill(hdc, x, y + 1, Cb, Cf, 6);
    RFloodFill(hdc, x, y - 1, Cb, Cf, 6);
    save(x, y);
}
struct Vertex
{
    double x, y;
    Vertex(int x1 = 0, int y1 = 0)
    {
        x = x1;
        y = y1;
    }
};
void NRFloodFill(HDC hdc, int x, int y, COLORREF Cb, COLORREF Cf, int n)
{
    std::stack<Vertex> S;
    S.push(Vertex(x, y));

    while (!S.empty())
    {
        Vertex v = S.top();
        S.pop();

        if (v.x < 0 || v.y < 0) continue;

        COLORREF c = GetPixel(hdc, v.x, v.y);

        if (c == Cb || c == Cf) continue;

        SetPixel(hdc, v.x, v.y, Cf);

        
        S.push(Vertex(v.x + 1, v.y));
        S.push(Vertex(v.x - 1, v.y));
        S.push(Vertex(v.x, v.y + 1));
        S.push(Vertex(v.x, v.y - 1));

        
    }
}
/**filling*/
struct Point
{
    int x, y;
    Point(int x =0 , int y =0) :x(x), y(y)
    {
    }
};

void FillingL(HDC hdc, int x, int y, COLORREF BC, COLORREF FC, int n, int R)
{
    stack <Point> st;
    st.push(Point(x, y));
    while (!st.empty())
    {
        Point p = st.top();
        st.pop();
        COLORREF c = GetPixel(hdc, p.x, p.y);

        // Optional: prevent overflow outside window
        if (p.x < 0 || p.x > 899 || p.y < 0 || p.y > 599) continue;

        // Optional: limit to the filled circle
        if (c == BC || c == FC || !insideCircle(p.x, p.y, x, y, R)) continue;

        SetPixel(hdc, p.x, p.y, FC);
        save(p.x, p.y);

        switch (n) {
        case 1: // Q1
             st.push(Point(p.x + 1, p.y));
             st.push(Point(p.x, p.y + 1));
            break;
        case 2: // Q2
             st.push(Point(p.x - 1, p.y));
             st.push(Point(p.x, p.y + 1));
            break;
        case 3: // Q3
             st.push(Point(p.x - 1, p.y));
             st.push(Point(p.x, p.y - 1));
            break;
        case 4: // Q4
             st.push(Point(p.x + 1, p.y));
             st.push(Point(p.x, p.y - 1));
            break;
        case 5: // Full
             st.push(Point(p.x + 1, p.y));
             st.push(Point(p.x - 1, p.y));
             st.push(Point(p.x, p.y + 1));
             st.push(Point(p.x, p.y - 1));
            break;
        }
    }
}


/**Filling by circle with other circle */
void Draw8Points2(HDC hdc, int xc, int yc, int x, int y, COLORREF c, int Q)
{
    if (Q == 1)
    {

        SetPixel(hdc, xc + x, yc - y, c);
        SetPixel(hdc, xc + y, yc - x, c);
        save(xc + x, yc - y);
        save(xc + y, yc - x);
    }
    else if (Q == 2)
    {
        SetPixel(hdc, xc - y, yc - x, c),
            SetPixel(hdc, xc - x, yc - y, c);
        save(xc - y, yc - x);
        save(xc - x, yc - y);

    }
    else if (Q == 3)
    {
        SetPixel(hdc, xc - x, yc + y, c);
        SetPixel(hdc, xc - y, yc + x, c);
        save(xc - x, yc + y);
        save(xc - y, yc + x);

    }
    else if (Q == 4)
    {
        SetPixel(hdc, xc + x, yc + y, c);
        SetPixel(hdc, xc + y, yc + x, c);
        save(xc + x, yc + y);
        save(xc + y, yc + x);
    }
    else if (Q == 5) {
        SetPixel(hdc, xc + x, yc - y, c);
        SetPixel(hdc, xc + y, yc - x, c);
        SetPixel(hdc, xc - y, yc - x, c);
        SetPixel(hdc, xc - x, yc - y, c);
        SetPixel(hdc, xc - x, yc + y, c);
        SetPixel(hdc, xc - y, yc + x, c);
        SetPixel(hdc, xc + x, yc + y, c);
        SetPixel(hdc, xc + y, yc + x, c);
        save(xc + x, yc + y);
        save(xc + y, yc + x);
        save(xc - x, yc + y);
        save(xc - y, yc + x);
        save(xc - y, yc - x);
        save(xc - x, yc - y);
        save(xc + x, yc - y);
        save(xc + y, yc - x);

    }
}




/**Ellipse*/

void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
    SetPixel(hdc, xc + x, yc + y, c);
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc - x, yc + y, c);
    SetPixel(hdc, xc - x, yc - y, c);

    // save(xc + x, yc + y);
    // save(xc + x, yc - y);
    // save(xc - x, yc + y);
    // save(xc - x, yc - y);
}

double dy_dx(int x, double A, double B) {
    double y = B * sqrt(1.0 - (pow(x, 2) / pow(A, 2)));
    return (-x * pow(B, 2)) / (y * pow(A, 2));
}

// Ellipse using direct equation
void Ellipse(HDC hdc, int xc, int yc, int A, int B, COLORREF c) {
    int x = 0, y = B;
    Draw4Points(hdc, xc, yc, x, y, c);

    while (x < A && y >= 0) {
        if (abs(dy_dx(x, A, B)) <= 1) {
            x++;
            y = int(B * sqrt(1.0 - (pow(x, 2) / pow(A, 2))));
            Draw4Points(hdc, xc, yc, x, y, c);
        }
        else {
            break;
        }
    }

    x = A;
    y = 0;
    Draw4Points(hdc, xc, yc, x, y, c);

    while (y < B && x >= 0) {
        if (abs(dy_dx(x, A, B)) > 1) {
            y++;
            x = int(A * sqrt(1.0 - (pow(y, 2) / pow(B, 2))));
            Draw4Points(hdc, xc, yc, x, y, c);
        }
        else {
            break;
        }
    }
}

// Ellipse using polar coordinates
void DrawEllipsePolar(HDC hdc, int xc, int yc, int A, int B, COLORREF c) {
    double dtheta = 1.0 / max(A, B);
    for (double theta = 0; theta < 2 * 3.14159265; theta += dtheta) {
        int x = Round(xc + A * cos(theta));
        int y = Round(yc + B * sin(theta));
        SetPixel(hdc, x, y, c);
        // save(x, y); // Optional
    }
}

// Ellipse using midpoint (simulated polar-like)
void midptellipse(HDC hdc, int xc, int yc, int A, int B, COLORREF c) {
    double dtheta = 1.0 / max(A, B);
    double cd = cos(dtheta), sd = sin(dtheta);
    double x = A, y = 0;

    for (double theta = 0; theta < 2 * 3.14159; theta += dtheta) {
        SetPixel(hdc, Round(xc + x), Round(yc + y), c);
        double xt = x * cd - y * sd;
        double yt = x * sd + y * cd;
        x = xt;
        y = yt;
    }
}

/**Polygon*/

struct Entry
{
    int xmin, xmax;
};

void InitEntries(Entry table[])
{
    for (int i = 0; i < MAXENTRIES; i++)
    {
        table[i].xmin = INT_MAX;
        table[i].xmax = -INT_MAX;
    }
}

void ScanEdge(POINT v1, POINT v2, Entry table[])
{
    if (v1.y == v2.y) return;
    if (v1.y > v2.y) swap(v1, v2);
    double minv = (double)(v2.x - v1.x) / (v2.y - v1.y);
    double x = v1.x;
    int y = v1.y;
    while (y < v2.y) {
        if (y >= 0 && y < MAXENTRIES) { // Bounds check
            if (x < table[y].xmin) table[y].xmin = (int)ceil(x);
            if (x > table[y].xmax) table[y].xmax = (int)floor(x);
        }
        y++;
        x += minv;
    }
}

void DrawSanLines(HDC hdc, Entry table[], COLORREF color)
{
    for (int y = 0; y < MAXENTRIES; y++)
        if (table[y].xmin < table[y].xmax)
            for (int x = table[y].xmin; x <= table[y].xmax; x++) {
                SetPixel(hdc, x, y, color);
                save(x, y);
            }
}

void ConvexFill(HDC hdc, POINT p[], int n, COLORREF color)
{
    Entry* table = new Entry[MAXENTRIES];
    InitEntries(table);
    POINT v1 = p[n - 1];
    for (int i = 0; i < n; i++) {
        POINT v2 = p[i];
        ScanEdge(v1, v2, table);
        v1 = p[i];
    }
    DrawSanLines(hdc, table, color);
    delete[] table;
}

struct EdgeRec
{
    double x;
    double minv;
    int ymax;
    bool operator<(EdgeRec r)
    {
        return x < r.x;
    }
};

typedef list<EdgeRec> EdgeList;

EdgeRec InitEdgeRec(POINT& v1, POINT& v2)
{
    if (v1.y > v2.y) swap(v1, v2);
    EdgeRec rec;
    rec.x = v1.x;
    rec.ymax = v2.y;
    rec.minv = (v2.x != v1.x) ? (double)(v2.x - v1.x) / (v2.y - v1.y) : 0; 
    return rec;
}

void InitEdgeTable(POINT* p, int n, EdgeList table[])
{
    POINT v1 = p[n - 1];
    for (int i = 0; i < n; i++)
    {
        POINT v2 = p[i];
        if (v1.y == v2.y) { v1 = v2; continue; }
        EdgeRec rec = InitEdgeRec(v1, v2);
        int y = v1.y;
        if (y >= 0 && y < MAXENTRIES) { // Bounds check
            cout << "Edge from (" << v1.x << ", " << v1.y << ") to (" << v2.x << ", " << v2.y <<
                ") added at y=" << y << ", x=" << rec.x << ", minv=" << rec.minv << endl;
            table[y].push_back(rec);
        }
        v1 = v2;
    }
}
#include <algorithm>
void GeneralPolygonFill(HDC hdc, POINT p[], int n, COLORREF c)
{
    EdgeList* table = new EdgeList[MAXENTRIES];
    InitEdgeTable(p, n, table);
    int y = 0;
    while (y < MAXENTRIES && table[y].size() == 0) y++;
    if (y == MAXENTRIES) {
        cout << "No valid scan lines found within MAXENTRIES range.\n";
        delete[] table;
        return;
    }
    EdgeList ActiveList = table[y];
    while (y < MAXENTRIES && !ActiveList.empty())
    {
        cout << "Processing y=" << y << ", ActiveList size=" << ActiveList.size() << endl;
        ActiveList.sort();
        vector<int> xIntersections;
        for (EdgeList::iterator it = ActiveList.begin(); it != ActiveList.end(); ++it)
        {
            xIntersections.push_back((int)ceil(it->x));
        }
        sort(xIntersections.begin(), xIntersections.end());
        for (size_t i = 0; i < xIntersections.size(); i += 2)
        {
            if (i + 1 < xIntersections.size())
            {
                int x1 = xIntersections[i];
                int x2 = xIntersections[i + 1];
                cout << "Filling from x=" << x1 << " to x=" << x2 << " at y=" << y << endl;
                for (int x = x1; x <= x2; x++)
                {
                    if (y >= 0 && y < MAXENTRIES && x >= 0 && x < 900) // Match window width
                    {
                        SetPixel(hdc, x, y, c);
                    }
                }
            }
        }
        y++;
        // Update ActiveList
        EdgeList::iterator it = ActiveList.begin();
        while (it != ActiveList.end())
        {
            if (y >= it->ymax) it = ActiveList.erase(it);
            else ++it;
        }
        for (EdgeList::iterator it_update = ActiveList.begin(); it_update != ActiveList.end(); ++it_update)
            it_update->x += it_update->minv;
        if (y < MAXENTRIES)
            ActiveList.insert(ActiveList.end(), table[y].begin(), table[y].end());
    }
    delete[] table;
}

//Clip Point 
void PointClippingC(HDC hdc, int x, int y, int xc, int yc, int r, COLORREF color) {
    if (insideCircle(x, y, xc, yc, r)) {
        SetPixel(hdc, x, y, color);
    }
}

void PointClipping(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom, COLORREF color)
{
    if (x >= xleft && x <= xright && y >= ytop && y <= ybottom)
        SetPixel(hdc, x, y, color);
}






////////////Clip Section ///////////////////////////////
void LineClippingC(HDC hdc, int x1, int y1, int x2, int y2, int xc, int yc, int r, COLORREF c) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    for (double t = 0;t < 1;t += 0.001) {
        int x = x1 + (dx * t);
        int y = y1 + (dy * t);
        if (insideCircle(x, y, xc, yc, r)) {
            SetPixel(hdc, x, y, c);
        }
    }
}

union OutCode {
    unsigned All : 4;
    struct { unsigned left : 1, top : 1, right : 1, bottom : 1; };
};

OutCode GetOutCode(double x, double y,
    int xLeft, int yTop, int xRight, int yBottom)
{
    OutCode out{ 0 };
    if (x < xLeft)        out.left = 1; else if (x > xRight)  out.right = 1;
    if (y < yTop)         out.top = 1; else if (y > yBottom) out.bottom = 1;
    return out;
}

static void VIntersect(double xs, double ys, double xe, double ye,
    int x, double& xi, double& yi)
{
    xi = x;
    yi = ys + (x - xs) * (ye - ys) / (xe - xs);
}

static void HIntersect(double xs, double ys, double xe, double ye,
    int y, double& xi, double& yi)
{
    yi = y;
    xi = xs + (y - ys) * (xe - xs) / (ye - ys);
}

void CohenSuth(HDC hdc, int xs, int ys, int xe, int ye, int xLeft, int yTop, int xRight, int yBottom, COLORREF c)
{
    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
    OutCode out1 = GetOutCode(x1, y1, xLeft, yTop, xRight, yBottom);
    OutCode out2 = GetOutCode(x2, y2, xLeft, yTop, xRight, yBottom);

    while ((out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi{}, yi{};
        if (out1.All)
        {
            if (out1.left)       VIntersect(x1, y1, x2, y2, xLeft, xi, yi);
            else if (out1.right) VIntersect(x1, y1, x2, y2, xRight, xi, yi);
            else if (out1.top)   HIntersect(x1, y1, x2, y2, yTop, xi, yi);
            else                 HIntersect(x1, y1, x2, y2, yBottom, xi, yi);
            x1 = xi; y1 = yi;
            out1 = GetOutCode(x1, y1, xLeft, yTop, xRight, yBottom);
        }
        else
        {
            if (out2.left)       VIntersect(x1, y1, x2, y2, xLeft, xi, yi);
            else if (out2.right) VIntersect(x1, y1, x2, y2, xRight, xi, yi);
            else if (out2.top)   HIntersect(x1, y1, x2, y2, yTop, xi, yi);
            else                 HIntersect(x1, y1, x2, y2, yBottom, xi, yi);
            x2 = xi; y2 = yi;
            out2 = GetOutCode(x2, y2, xLeft, yTop, xRight, yBottom);
        }
    }

    if (!out1.All && !out2.All)
        DDALine(hdc, Round(x1), Round(y1), Round(x2), Round(y2), c);
}

typedef vector<Vertex> VertexList;
typedef bool (*IsInFunc)(Vertex& v, int edge);
typedef Vertex(*IntersectFunc)(Vertex& v1, Vertex& v2, int edge);

VertexList ClipWithEdge(VertexList p, int edge, IsInFunc In, IntersectFunc Intersect)
{
    VertexList OutList;
    Vertex v1 = p[p.size() - 1];
    bool v1_in = In(v1, edge);
    for (int i = 0;i < (int)p.size();i++)
    {
        Vertex v2 = p[i];
        bool v2_in = In(v2, edge);
        if (!v1_in && v2_in)
        {
            OutList.push_back(Intersect(v1, v2, edge));
            OutList.push_back(v2);
        }
        else if (v1_in && v2_in) OutList.push_back(v2);
        else if (v1_in) OutList.push_back(Intersect(v1, v2, edge));
        v1 = v2;
        v1_in = v2_in;
    }
    return OutList;
}

bool InLeft(Vertex& v, int edge)
{
    return v.x >= edge;
}
bool InRight(Vertex& v, int edge)
{
    return v.x <= edge;
}
bool InTop(Vertex& v, int edge)
{
    return v.y >= edge;
}
bool InBottom(Vertex& v, int edge)
{
    return v.y <= edge;
}
Vertex VIntersect(Vertex& v1, Vertex& v2, int xedge)
{
    Vertex res;
    res.x = xedge;
    res.y = v1.y + (xedge - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
    return res;
}
Vertex HIntersect(Vertex& v1, Vertex& v2, int yedge)
{
    Vertex res;
    res.y = yedge;
    res.x = v1.x + (yedge - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
    return res;
}
void PolygonClip(HDC hdc, POINT* p, int n, int xleft, int ytop, int xright, int ybottom, COLORREF c)
{
    VertexList vlist;
    for (int i = 0;i < n;i++)vlist.push_back(Vertex(p[i].x, p[i].y));
    vlist = ClipWithEdge(vlist, xleft, InLeft, VIntersect);
    vlist = ClipWithEdge(vlist, ytop, InTop, HIntersect);
    vlist = ClipWithEdge(vlist, xright, InRight, VIntersect);
    vlist = ClipWithEdge(vlist, ybottom, InBottom, HIntersect);
    Vertex v1 = vlist[vlist.size() - 1];
    for (int i = 0;i < (int)vlist.size();i++)
    {
        Vertex v2 = vlist[i];
        MoveToEx(hdc, Round(v1.x), Round(v1.y), NULL);
        LineTo(hdc, Round(v2.x), Round(v2.y));
        v1 = v2;
    }
}


/**Cardinal Spline Curve*/

static Vector2 ps[4];
class Vector4
{
    double v[4];
public:
    Vector4(double a = 0, double b = 0, double c = 0, double d = 0)
    {
        v[0] = a; v[1] = b; v[2] = c; v[3] = d;
    }
    Vector4(double a[])
    {
        memcpy(v, a, 4 * sizeof(double));
    }
    double& operator[](int i)
    {
        return v[i];
    }
};
class Matrix4
{
    Vector4 M[4];
public:
    Matrix4(double A[])
    {
        memcpy(M, A, 16 * sizeof(double));
    }
    Vector4& operator[](int i)
    {
        return M[i];
    }
};


Vector4 operator*(Matrix4 M, Vector4& b) // right multiplication of M by b
{
    Vector4 res;
    for (int i = 0;i < 4;i++)
        for (int j = 0;j < 4;j++)
            res[i] += M[i][j] * b[j];
    return res;
}

double DotProduct(Vector4& a, Vector4& b) //multiplying a raw vector by a column vector
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}


Vector4 GetHermiteCoeff(double x0, double s0, double x1, double s1)
{
    static double H[16] = { 2,1,-2,1,-3,-2,3,-1,0,1,0,0,1,0,0,0 };
    static Matrix4 basis(H);
    Vector4 v(x0, s0, x1, s1);
    return basis * v;
}


void DrawHermiteCurve(HDC hdc, Vector2& P0, Vector2& T0, Vector2& P1, Vector2& T1, int numpoints, COLORREF color)
{
    Vector4 xcoeff = GetHermiteCoeff(P0.x, T0.x, P1.x, T1.x);
    Vector4 ycoeff = GetHermiteCoeff(P0.y, T0.y, P1.y, T1.y);
    if (numpoints < 2)return;
    double dt = 1.0 / (numpoints - 1);
    for (double t = 0;t <= 1;t += dt)
    {
        Vector4 vt;
        vt[3] = 1;
        for (int i = 2;i >= 0;i--)vt[i] = vt[i + 1] * t;
        int x = round(DotProduct(xcoeff, vt));
        int y = round(DotProduct(ycoeff, vt));
        SetPixel(hdc, x, y, color);
    }
}

void DrawHermiteCurve(HDC hdc, Vector2& P0, Vector2& T0, Vector2& P1, Vector2& T1, int numpoints)
{
    Vector4 xcoeff = GetHermiteCoeff(P0.x, T0.x, P1.x, T1.x);
    Vector4 ycoeff = GetHermiteCoeff(P0.y, T0.y, P1.y, T1.y);
    if (numpoints < 2)return;
    double dt = 1.0 / (numpoints - 1);
    for (double t = 0;t <= 1;t += dt)
    {
        Vector4 vt;
        vt[3] = 1;
        for (int i = 2;i >= 0;i--)vt[i] = vt[i + 1] * t;
        int x = round(DotProduct(xcoeff, vt));
        int y = round(DotProduct(ycoeff, vt));
        if (t == 0)MoveToEx(hdc, x, y, NULL);else LineTo(hdc, x, y);
    }
}

void DrawBezierCurve(HDC hdc, Vector2& P0, Vector2& P1, Vector2& P2, Vector2& P3, int numpoints)
{
    Vector2 T0(3 * (P1.x - P0.x), 3 * (P1.y - P0.y));
    Vector2 T1(3 * (P3.x - P2.x), 3 * (P3.y - P2.y));
    DrawHermiteCurve(hdc, P0, T0, P3, T1, numpoints);
}

void DrawCardinalSpline(HDC hdc, Vector2 P[], int n, double c, int numpix)
{
    double c1 = 1 - c;
    Vector2 T0(c1 * (P[2].x - P[0].x), c1 * (P[2].y - P[0].y));
    for (int i = 2;i < n - 1;i++)
    {
        Vector2 T1(c1 * (P[i + 1].x - P[i - 1].x), c1 * (P[i + 1].y - P[i - 1].y));
        DrawHermiteCurve(hdc, P[i - 1], T0, P[i], T1, numpix);
        T0 = T1;
    }
}
//m and n points yara                                                                                                                                                                                         }
void DrawHermiteCurvey(HDC hdc, Vector& p1, Vector& T1, Vector& p2, Vector& T2, COLORREF c)
{
    double a0 = p1[0], a1 = T1[0],
        a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
        a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];
    double b0 = p1[1], b1 = T1[1],
        b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
        b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];
    for (double t = 0; t <= 1; t += 0.001)
    {
        double t2 = t * t, t3 = t2 * t;
        double x = a0 + a1 * t + a2 * t2 + a3 * t3;
        double y = b0 + b1 * t + b2 * t2 + b3 * t3;
        SetPixel(hdc, Round(x), Round(y), c);
    }
}

void DrawBezierCurvey(HDC hdc, Vector& P0, Vector& P1, Vector& P2, Vector& P3, COLORREF color)
{
    Vector T0, T1;
    T0[0] = (P1[0] - P0[0]);
    T0[1] = (P1[1] - P0[1]);
    T1[0] = (P3[0] - P2[0]);
    T1[1] = (P3[1] - P2[1]);
    DrawHermiteCurvey(hdc, P0, T0, P3, T1, color);
}
void swap(int& x1, int& y1, int& x2, int& y2)
{
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}

void Line(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dy) <= abs(dx))
    {
        if (x1 > x2)swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, c);
        int x = x1;
        while (x < x2)
        {
            x++;
            double y = y1 + (double)(x - x1) * dy / dx;
            SetPixel(hdc, x, Round(y), c);
        }
    }
    else {
        if (y1 > y2)swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, c);
        int y = y1;
        while (y < y2)
        {
            y++;
            double x = x1 + (double)(y - y1) * dx / dy;
            SetPixel(hdc, Round(x), y, c);
        }
    }

}
void squaring(int& x1, int& y1, int& x2, int& y2) {

    int dis1 = abs(x1 - x2);
    int dis2 = abs(y1 - y2);
    if (dis1 > dis2) {
        if (y2 < y1) y2 = y1 + dis1;
        else y1 = y2 + dis1;
    }
    else {
        if (x2 < x1) x2 = x1 + dis2;
        else x1 = x2 + dis2;

    }
}
static int x_left, x_right, y_top, y_bottom;
void DrawSquare(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    squaring(x1, y1, x2, y2);
    Line(hdc, x1, y1, x2, y1, RGB(255, 0, 0));
    Line(hdc, x2, y1, x2, y2, RGB(255, 0, 0));
    Line(hdc, x2, y2, x1, y2, RGB(255, 0, 0));
    Line(hdc, x1, y2, x1, y1, RGB(255, 0, 0));
    x_left = min(x1, x2); x_right = max(x1, x2);
    y_top = max(y1, y2); y_bottom = min(y1, y2);
}
//static int x_left,x_right,y_top,y_bottom;
void Drawrectangley(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    //squaring(x1, y1, x2, y2);
    Line(hdc, x1, y1, x2, y1, RGB(255, 0, 0));
    Line(hdc, x2, y1, x2, y2, RGB(255, 0, 0));
    Line(hdc, x2, y2, x1, y2, RGB(255, 0, 0));
    Line(hdc, x1, y2, x1, y1, RGB(255, 0, 0));
    x_left = min(x1, x2); x_right = max(x1, x2);
    y_top = max(y1, y2); y_bottom = min(y1, y2);
}
void fillhorizontalwithbezeir(HDC hdc, int x_left, int x_right, int y_top, int y_bottom, COLORREF C)
{
    int x1 = x_left + 1;
    int x2 = x_right - 1;
    int y1 = y_top; // Start from the top
    int y2 = y_bottom; // End at the bottom

    // Ensure coordinates are in the correct order (x1 <= x2, y1 <= y2)
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(y1, y2);

    // Adjust to stay within window bounds (assuming 900x600 window)
    x1 = max(0, x1);
    x2 = min(899, x2); // Window width is 900 pixels
    y1 = max(0, y1);
    y2 = min(599, y2); // Window height is 600 pixels

    cout << "Filling rectangle from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ")" << endl;

    // Fill from top to bottom
    for (int y = y1; y <= y2; y++)
    {
        for (int x = x1; x <= x2; x++)
        {
            SetPixel(hdc, x, y, C);
        }
        cout << "Filled row at y=" << y << endl;
    }
}

void fillverticalwithhermit(HDC hdc, int x_left, int x_right, int y_top, int y_bottom, COLORREF C)
{
    Vector p1;
    Vector p2;
    Vector p3;
    int x1 = x_left + 1;
    int x2 = x_right;
    int y1 = y_top - 1;
    int y2 = y_bottom + 1;
    p1 = Vector(x1, y1);
    p2 = Vector(x2, y1);
    p3 = Vector(0, 0);
    while (x1 < x2)
    {
        p1 = Vector(x1, y1);
        p2 = Vector(x1, y2);
        DrawHermiteCurvey(hdc, p1, p3, p2, p3, C);
        x1++;
    }

}


struct PointSquare {
    int x;
    int y;
};
int myMin(int a, int b) {
    return (a < b) ? a : b;
}




//////Filling by circle

void FillCircleWithSmallCircles(HDC hdc, int xc, int yc, int R, int r, COLORREF color, int quarter) {
    // r is the radius of the small circles (e.g., R/10 for reasonable size)
    // Ensure r is reasonable
    if (r <= 0 || r >= R) r = R / 10;

    // Draw the outer circle
    Directcircle(hdc, xc, yc, R, color);

    // Iterate over a grid to place small circles
    for (int x = xc - R; x <= xc + R; x +=(int) (1.3 * r)) {
        for (int y = yc - R; y <= yc + R; y += (int)(1.3 * r)) {
            // Check if the center of the small circle is within the large circle
            double distance = sqrt(pow(x - xc, 2) + pow(y - yc, 2));
            if (distance + r <= R) { // Small circle must be fully inside large circle
                bool draw = false;
                switch (quarter) {
                case 1: // Q1: x >= xc, y <= yc
                    draw = (x >= xc && y <= yc);
                    break;
                case 2: // Q2: x <= xc, y <= yc
                    draw = (x <= xc && y <= yc);
                    break;
                case 3: // Q3: x <= xc, y >= yc
                    draw = (x <= xc && y >= yc);
                    break;
                case 4: // Q4: x >= xc, y >= yc
                    draw = (x >= xc && y >= yc);
                    break;
                case 5: // Full circle
                    draw = true;
                    break;
                }
                if (draw) {
                    Directcircle(hdc, x, y, r, color);
                    FillingL(hdc, x, y, color, color, 5, r); // Full circle fill for small circle
                }
            }
        }
    }
}


/// Cardinal line 
double HermitMatrix[4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {-3, -2, 3, -1},
    {2, 1, -2, 1}
};
void hermitCurve(HDC hdc, Point p1, Point t1, Point p2, Point t2, COLORREF color) {
    Point arr[4] = { p1, t1, p2, t2 };
    double alpha[4]{}, beta[4]{};

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            alpha[i] += HermitMatrix[i][j] * arr[j].x;
            beta[i] += HermitMatrix[i][j] * arr[j].y;
        }
    }

    for (double t = 0; t <= 1; t += 0.001) {
        double x = 0, y = 0;
        for (int i = 0; i < 4; ++i) {
            x += alpha[i] * pow(t, i);
            y += beta[i] * pow(t, i);
        }
        SetPixel(hdc, round(x), round(y), color);
    }
}

// Cardinal spline using Hermite
void cardinalSpline(HDC hdc, Point p[], int n, double c , COLORREF color) {
    Point tension[100]; // enough space
    for (int i = 1; i < n - 1; ++i) {
        tension[i].x = c * (p[i + 1].x - p[i - 1].x);
        tension[i].y = c * (p[i + 1].y - p[i - 1].y);
    }
    tension[0].x = c * (p[1].x - p[0].x);
    tension[0].y = c * (p[1].y - p[0].y);
    tension[n - 1].x = c * (p[n - 1].x - p[n - 2].x);
    tension[n - 1].y = c * (p[n - 1].y - p[n - 2].y);

    for (int i = 0; i < n - 1; ++i) {
        hermitCurve(hdc, p[i], tension[i], p[i + 1], tension[i + 1], color);
    }
}



void clearScreen() {
#ifdef WINDOWS
    system("cls");
#else
    system("cls");
#endif
}

int X1, Y1, x2, y2, xc, yc, Y, k, nume, R, R2, Rc, xf, yf, match, num, index, xl, yt, xr, yb, xf2, yf2, d, Q, y;
static Point points1[6];
static int clickCount = 0;

int Num_of_Points = 0;
static Vector2 points[5];
POINT P[5];
double minv;
int counter = 0;
static Vector s[2];
int counter5 = 0;
static POINT rectStart{}, rectEnd{};
static RECT  clipRect{};
static bool  drawingRect = false;
static bool  rectReady = false;

static POINT lineStart{};              // first click of segment
static bool  haveLineStart = false;    // waiting for second click




// Colors
#define ID_COLOR_RED 1
#define ID_COLOR_BLUE 2
#define ID_COLOR_GREEN 3
#define ID_COLOR_YELLOW 4
#define ID_COLOR_ORANGE 5
#define ID_COLOR_BLACK 6
#define ID_COLOR_PURPLE 7
#define ID_COLOR_GREY 8

// Filling
#define ID_FILL_Q1 10
#define ID_FILL_Q2 11
#define ID_FILL_Q3 12
#define ID_FILL_Q4 13
#define ID_FILL_FULL 14
#define ID_FILL_RECURSIVE 15
#define ID_FILL_NONRECURSIVE 16

// File
#define ID_SAVE 20
#define ID_LOAD 21

// Circle Algorithms
#define ID_CIRCLE_DIRECT 30
#define ID_CIRCLE_POLAR 31
#define ID_CIRCLE_ITERATIVE 32
#define ID_CIRCLE_MIDPOINT 33
#define ID_CIRCLE_MODIFIED 34

// Line Algorithms
#define ID_LINE_DDA 40
#define ID_LINE_MIDPOINT 41
#define ID_LINE_PARAMETRIC 42

// Clear
#define ID_CLEAR_SCREEN 50

// Clipping
#define ID_CLIP_RECT 60
#define ID_CLIP_CIRCLE 61
#define ID_CLIP_SQUARE 62
#define ID_CLIP_LINE 63
#define ID_CLIP_POINT 64
#define ID_CLIP_POLYGON 65

// Ellipse
#define ID_ELLIPSE_DIRECT 70
#define ID_ELLIPSE_POLAR 71
#define ID_ELLIPSE_MID 72

// Polygon
#define ID_POLYGON_CONVEX 80
#define ID_POLYGON_NONCONVEX 81

// Circle Filling with Circle
#define ID_CIRCLE_Q1 90
#define ID_CIRCLE_Q2 91
#define ID_CIRCLE_Q3 92
#define ID_CIRCLE_Q4 93
#define ID_CIRCLE_FULL 94

// Curves
#define ID_CARDINAL 100
#define ID_FILL_SQUARE_CURVE 101
#define ID_FILL_RECT_BEZIER 102

#define ID_FILL_CIRCLE_Q1 50
#define ID_FILL_CIRCLE_Q2 51
#define ID_FILL_CIRCLE_Q3 52
#define ID_FILL_CIRCLE_Q4 53
#define ID_FILL_CIRCLE_FULL 54

#define ID_INPUT_LINE 200



LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    string line;
    vector<int> vect;
    ifstream in;

    HDC hdc = GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {


        //menu
    case WM_CREATE:
        AddMenus(hwnd);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
            /**colors*/
        case ID_COLOR_RED:
            Y = RGB(255, 0, 0);
            cout << "color=Red" << endl;
            break;
        case ID_COLOR_BLUE:
            Y = RGB(0, 0, 255);
            break;
        case ID_COLOR_GREEN:
            Y = RGB(0, 255, 0);
            cout << "color=Green" << endl;
            break;
        case ID_COLOR_YELLOW:
            Y = RGB(255, 255, 0);
            cout << "color=Yellow" << endl;
            break;
        case ID_COLOR_ORANGE:
            Y = RGB(255, 128, 0);
            cout << "color=Orange" << endl;
            break;
        case ID_COLOR_BLACK:
            Y = RGB(0, 0, 0);
            cout << "color=Black" << endl;
            break;
        case ID_COLOR_PURPLE:
            Y = RGB(204, 0, 204);
            cout << "color=Purple" << endl;
            break;
        case ID_COLOR_GREY:
            Y = RGB(160, 160, 160);
            cout << "color=Grey" << endl;
            break;

            /**filling*/
        case ID_FILL_Q1:
            nume = 1;
            cout << "Filling-> first quarter \n";
            break;
        case ID_FILL_Q2:
            nume = 2;
            cout << "Filling-> second quarter \n";
            break;
        case ID_FILL_Q3:
            nume = 3;
            cout << "Filling-> third quarter \n";
            break;
        case ID_FILL_Q4:
            nume = 4;
            cout << "Filling-> fourth quarter \n";
            break;
        case ID_FILL_FULL:
            nume = 5;
            cout << "Filling-> Full quarter \n";
            break;
        case ID_FILL_RECURSIVE:
            nume = 6;
            cout << "Recursive Floodfill \n";
            break;
        case ID_FILL_NONRECURSIVE:
            nume = 7;
            cout << "Non-Recursive Floodfill \n";
            break;

            /**save*/
        case ID_SAVE:
            SaveInFile();
            cout << "Saved\n";
            break;

            /**load*/
        case ID_LOAD:
            in.open("save.txt");
            getline(in, line);
            read(vect, line);
            hdc = GetDC(hwnd);
            savedPoints = vect;
            prev(hdc, vect, Y);
            in.close();
            break;

            /**circle*/
        case ID_CIRCLE_DIRECT:
            match = 1;
            cout << "Circle Direct\n";
            break;
        case ID_CIRCLE_POLAR:
            match = 2;
            cout << "Circle Polar\n";
            break;
        case ID_CIRCLE_ITERATIVE:
            match = 3;
            cout << "Circle Iterative\n";
            break;
        case ID_CIRCLE_MIDPOINT:
            match = 4;
            cout << "Circle Midpoint\n";
            break;
        case ID_CIRCLE_MODIFIED:
            match = 5;
            cout << "Circle Modified\n";
            break;

            /**line*/
        case ID_LINE_DDA:
            match = 6;
            cout << "Line DDA\n";
            break;
        case ID_LINE_MIDPOINT:
            match = 7;
            cout << "Line Midpoint\n";
            break;
        case ID_LINE_PARAMETRIC:
            match = 8;
            cout << "Line Parametric\n";
            break;

            /**clear screen*/
        case ID_CLEAR_SCREEN:
            ShowWindow(hwnd, SW_HIDE);
            ShowWindow(hwnd, SW_RESTORE);
            clearScreen();
            cout << "Screen cleared\n";
            break;

            /**clipping*/
        case ID_CLIP_RECT:
            num = 1;
            cout << "Rectangle clipping window\n";
            // Draw the fixed rectangle immediately
            if (!rectReady)
            {
                hdc = GetDC(hwnd);
                x_left = 100;
                y_top = 100;
                x_right = 600;
                y_bottom = 400;
                Rectangle(hdc, x_left, y_top, x_right, y_bottom);
                rectReady = true;
                ReleaseDC(hwnd, hdc);
            }
            break;
        case ID_CLIP_CIRCLE:
            num = 2;
            break;
        case ID_CLIP_SQUARE:
            num = 3;
            if (!rectReady)
            {
                hdc = GetDC(hwnd);
                x_left = 150;
                y_top = 150;
                x_right = 350;
                y_bottom = 350;
                Rectangle(hdc, x_left, y_top, x_right, y_bottom); // or call DrawSquare if needed
                rectReady = true;
                ReleaseDC(hwnd, hdc);
            }
            cout << "Square clipping window\n";
            break;
        case ID_CLIP_LINE:
            match = 10;
            cout << "Clip line\n";
            break;
        case ID_CLIP_POINT:
            match = 11;
            cout << "Clip point\n";
            break;
        case ID_CLIP_POLYGON:
            match = 16;
            cout << "Clip polygon\n";
            break;

            /**ellipse*/
        case ID_ELLIPSE_DIRECT:
            match = 12;
            cout << "Ellipse Direct\n";
            break;
        case ID_ELLIPSE_POLAR:
            match = 13;
            cout << "Ellipse Polar\n";
            break;
        case ID_ELLIPSE_MID:
            match = 14;
            cout << "Ellipse Midpoint\n";

            break;

            /**polygon*/
        case ID_POLYGON_CONVEX:
            match = 15;
            cout << "Convex Fill Polygon\n";
            break;
        case ID_POLYGON_NONCONVEX:
            match = 26;
            cout << "Non-Convex Polygon\n";
            break;

            /**circle with other circle*/

        case ID_CIRCLE_Q1:
            nume = 17;
            cout << "Fill Circle with Circle -> Q1\n";
            break;
        case ID_CIRCLE_Q2:
            nume = 18;
            cout << "Fill Circle with Circle -> Q2\n";
            break;
        case ID_CIRCLE_Q3:
            nume = 19;
            cout << "Fill Circle with Circle -> Q3\n";
            break;
        case ID_CIRCLE_Q4:
            nume = 20;
            cout << "Fill Circle with Circle -> Q4\n";
            break;
        case ID_CIRCLE_FULL:
            nume = 21;
            cout << "Fill Circle with Circle -> Full\n";
            break;

            /**cardinal and bezier filling*/
        case ID_CARDINAL:
            match = 22;
            cout << "Cardinal Spline Curve\n";
            break;
        case ID_FILL_SQUARE_CURVE:
            match = 23;
            cout << "Filling Square with Curve\n";
            break;
        case ID_FILL_RECT_BEZIER:
            match = 24;
            cout << "Filling Rectangle with Bezier\n";
            break;
        
        case ID_INPUT_LINE:
        
            int x1, y1, x2, y2;
            cout << "Enter x1, y1, x2, y2: ";
            cin >> x1 >> y1 >> x2 >> y2;

            HDC hdc = GetDC(hwnd);
            DDALine(hdc, x1, y1, x2, y2, Y); // Or any algorithm you like
            ReleaseDC(hwnd, hdc);
            break;
        }


    case WM_LBUTTONDBLCLK:
        xl = LOWORD(lParam);
        yt = HIWORD(lParam);

        break;
    case WM_RBUTTONDBLCLK:

        xr = LOWORD(lParam);
        yb = HIWORD(lParam);
        R = Round(sqrt(pow(xr - xl, 2.0) + pow(yb - yt, 2.0)));
        cout << "(" << xl << " , " << yt << ") , (" << xr << " , " << yb << ")" << endl;

        if (num == 1)
        {
            Rectangle(hdc, xl, yt, xr, yb);
        }
        if (num == 2) {
            Directcircle(hdc, xl, yt, R, Y);
        }
        if (num == 3) {
            Rectangle(hdc, xr, yb, xr + 150, yb + 150);
        }
        d = Round(R / sqrt(2));
        Rc = R;
        xf = xl;
        yf = yt;
        xf2 = xr;
        yf2 = yb;
        break;


    case WM_LBUTTONDOWN:
        xc = LOWORD(lParam);
        yc = HIWORD(lParam);
        X1 = LOWORD(lParam);
        Y1 = HIWORD(lParam);

        if (match == 22) {

            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            points1[clickCount++] = Point(x, y);

            SetPixel(hdc, x, y, RGB(0, 0, 255));

            if (clickCount == 6) {
                cout << "Clicked: " << clickCount << " at (" << x << ", " << y << ")" << endl;
                cardinalSpline(hdc, points1, 6, 0.4,Y);
                clickCount = 0;
            }
            break;
        }


        if (match == 11 && num == 1)
        {
            if (!rectReady)
            {
                x_left = 150;
                y_top = 150;
                x_right = 350; // Side length of 200 (150 + 200)
                y_bottom = 350;
                Rectangle(hdc, x_left, y_top, x_right, y_bottom); // Draw the square
                rectReady = true; // Mark square as drawn
            }

            // Step 1: Draw the original point in gray to show the unclipped location
            COLORREF unclippedColor = RGB(255, 255, 255); // Gray color for unclipped point
            SetPixel(hdc, X1, Y1, unclippedColor);

            // Step 2: Clip the point and draw in selected color if inside the square
            PointClipping(hdc, X1, Y1, x_left, y_top, x_right, y_bottom, Y);

            cout << "Point drawn at (" << X1 << ", " << Y1 << ")" << endl;
            cout << "Clipping square: (" << x_left << ", " << y_top << ") to (" << x_right << ", " << y_bottom << ")" << endl;
        }
        if (match == 11 && num == 2)
        {
            if (!rectReady)
            {
                x_left = 350;
                y_top = 350;
                x_right = 350; // Side length of 200 (150 + 200)
                y_bottom = 350;
                Rectangle(hdc, x_left, y_top, x_right, y_bottom); // Draw the square
                rectReady = true; // Mark square as drawn
            }

            // Step 1: Draw the original point in gray to show the unclipped location
            COLORREF unclippedColor = RGB(255, 255, 255); // Gray color for unclipped point
            SetPixel(hdc, X1, Y1, unclippedColor);

            // Step 2: Clip the point and draw in selected color if inside the square
            PointClipping(hdc, X1, Y1, x_left, y_top, x_right, y_bottom, Y);

            cout << "Point drawn at (" << X1 << ", " << Y1 << ")" << endl;
            cout << "Clipping square: (" << x_left << ", " << y_top << ") to (" << x_right << ", " << y_bottom << ")" << endl;
        }
        if (match == 11 && num == 3)
        {
            // Draw the fixed square if not already drawn
            if (!rectReady)
            {
                x_left = 150;  // Fixed square coordinates (top-left corner)
                y_top = 150;
                x_right = 350; // Side length of 200 (150 + 200)
                y_bottom = 350;
                Rectangle(hdc, x_left, y_top, x_right, y_bottom); // Draw the square
                rectReady = true; // Mark square as drawn
            }

            // Step 1: Draw the original point in gray to show the unclipped location
            COLORREF unclippedColor = RGB(160, 160, 160); // Gray color for unclipped point
            SetPixel(hdc, X1, Y1, unclippedColor);

            // Step 2: Clip the point and draw in selected color if inside the square
            PointClipping(hdc, X1, Y1, x_left, y_top, x_right, y_bottom, Y);

            cout << "Point drawn at (" << X1 << ", " << Y1 << ")" << endl;
            cout << "Clipping square: (" << x_left << ", " << y_top << ") to (" << x_right << ", " << y_bottom << ")" << endl;
        }
        break;



    case WM_RBUTTONDOWN:
        x2 = LOWORD(lParam);
        y2 = HIWORD(lParam);
        R = Round(sqrt(pow(x2 - xc, 2.0) + pow(y2 - yc, 2.0)));

        if (nume >= 17 && nume <= 21) {
            int quarter;
            switch (nume) {
            case 17: quarter = 1; break; // Q1
            case 18: quarter = 2; break; // Q2
            case 19: quarter = 3; break; // Q3
            case 20: quarter = 4; break; // Q4
            case 21: quarter = 5; break; // Full
            }
            FillCircleWithSmallCircles(hdc, xc, yc, R, R / 10, Y, quarter);
            nume = 0; // Reset nume after drawing
            break;
        }

        //Circles Quarters filiing
       
        
       
      
       //Filiing match 
        if (match == 1) { // ID_CIRCLE_DIRECT
            Directcircle(hdc, xc, yc, R, Y);
            if (nume >= 1 && nume <= 5) {
                FillingL(hdc, xc, yc, Y, Y, nume, R); // Quadrant fill
            }
            else if (nume == 6) {
                RFloodFill(hdc, xc, yc, Y, Y, 6);
            }
            else if (nume == 7) {
                NRFloodFill(hdc, xc, yc, Y, Y, 7);
           
            }
            nume = 0;
            match = 0;
        }
        if (match == 2) { // ID_CIRCLE_DIRECT
            Directcircle(hdc, xc, yc, R, Y);
            if (nume >= 1 && nume <= 5) {
                FillingL(hdc, xc, yc, Y, Y, nume, R); // Quadrant fill
            }
            else if (nume == 6) {
                RFloodFill(hdc, xc, yc, Y, Y, 6);
            }
            else if (nume == 7) {
                NRFloodFill(hdc, xc, yc, Y, Y, 7);

            }
            nume = 0;
            match = 0;
        }

      

            if (match == 3)
            {
                itreativepolar(hdc, xc, yc, R, Y);
                if (nume == 1) {
                    FillingL(hdc, xc, yc, Y, Y, 1, R);
                }
                else if (nume == 2) {
                    FillingL(hdc, xc, yc, Y, Y, 2, R);
                }
                else if (nume == 3) {
                    FillingL(hdc, xc, yc, Y, Y, 3, R);
                }
                else if (nume == 4) {
                    FillingL(hdc, xc, yc, Y, Y, 4, R);
                }
                else if (nume == 5) {
                    FillingL(hdc, xc, yc, Y, Y, 5, R);
                }
                else if (nume == 6) {
                    RFloodFill(hdc, xc, yc, Y, Y, 6);
                }
                else if (nume == 7) {
                    NRFloodFill(hdc, xc, yc, Y, Y, 7);
                }
                
                nume = 0;
                match = 0;
            }

            if (match == 4)
            {
                midpointCircle(hdc, xc, yc, R, Y);

                if (nume == 1) {
                    FillingL(hdc, xc, yc, Y, Y, 1, R);
                }
                else if (nume == 2) {
                    FillingL(hdc, xc, yc, Y, Y, 2, R);
                }
                else if (nume == 3) {
                    FillingL(hdc, xc, yc, Y, Y, 3, R);
                }
                else if (nume == 4) {
                    FillingL(hdc, xc, yc, Y, Y, 4, R);
                }
                else if (nume == 5) {
                    FillingL(hdc, xc, yc, Y, Y, 5, R);
                }
                else if (nume == 6) {
                    RFloodFill(hdc, xc, yc, Y, Y, 6);
                }
                else if (nume == 7) {
                    NRFloodFill(hdc, xc, yc, Y, Y, 7);
                }
                nume = 0;
                match = 0;
            }

            if (match == 5)
            {
                modifiedMidpoint(hdc, xc, yc, R, Y);
                if (nume == 1) {
                    FillingL(hdc, xc, yc, Y, Y, 1, R);
                }
                else if (nume == 2) {
                    FillingL(hdc, xc, yc, Y, Y, 2, R);
                }
                else if (nume == 3) {
                    FillingL(hdc, xc, yc, Y, Y, 3, R);
                }
                else if (nume == 4) {
                    FillingL(hdc, xc, yc, Y, Y, 4, R);
                }
                else if (nume == 5) {
                    FillingL(hdc, xc, yc, Y, Y, 5, R);
                }
                else if (nume == 6) {
                    RFloodFill(hdc, xc, yc, Y, Y, 6);
                }
                else if (nume == 7) {
                    NRFloodFill(hdc, xc, yc, Y, Y, 7);
                }
                
                nume = 0;
                match = 0;
            }

            if (match == 6)
            {
                DDALine(hdc, X1, Y1, x2, y2, Y);

            }
            if (match == 7)
            {
                midpoint(hdc, X1, Y1, x2, y2, Y);

            }
            if (match == 8)
            {
                parametric(hdc, X1, Y1, x2, y2, Y);
                cout << X1 << " " << Y1 << " " << x2 << " " << y2 << endl;

            }
            ///Match Clip section
            if (match == 10 && num == 1)
            {
                if (!rectReady)
                {
                    x_left = 100;  // Fixed rectangle coordinates
                    y_top = 100;
                    x_right = 300;
                    y_bottom = 300;
                    Rectangle(hdc, x_left, y_top, x_right, y_bottom);
                    rectReady = true; // Mark rectangle as drawn
                }

                // Step 1: Draw the original line in gray to show the unclipped portion
                COLORREF unclippedColor = RGB(255, 255, 255); // Gray color for unclipped portion
                DDALine(hdc, X1, Y1, x2, y2, unclippedColor);

                // Step 2: Clip the line using Cohen-Sutherland and draw the clipped portion in the selected color
                CohenSuth(hdc, X1, Y1, x2, y2, x_left, y_top, x_right, y_bottom, Y);

                cout << "Line drawn from (" << X1 << ", " << Y1 << ") to (" << x2 << ", " << y2 << ")" << endl;
                cout << "Clipping rectangle: (" << x_left << ", " << y_top << ") to (" << x_right << ", " << y_bottom << ")" << endl;


            }
            if (match == 10 && num == 2)
            {
                if (!rectReady)
                {
                    x_left = 300;  // Fixed rectangle coordinates
                    y_top = 300;
                    x_right = 300;
                    y_bottom = 300;
                    Rectangle(hdc, x_left, y_top, x_right, y_bottom);
                    rectReady = true; // Mark rectangle as drawn
                }

                // Step 1: Draw the original line in gray to show the unclipped portion
                COLORREF unclippedColor = RGB(255, 255, 255); // Gray color for unclipped portion
                DDALine(hdc, X1, Y1, x2, y2, unclippedColor);

                // Step 2: Clip the line using Cohen-Sutherland and draw the clipped portion in the selected color
                CohenSuth(hdc, X1, Y1, x2, y2, x_left, y_top, x_right, y_bottom, Y);

                cout << "Line drawn from (" << X1 << ", " << Y1 << ") to (" << x2 << ", " << y2 << ")" << endl;
                cout << "Clipping rectangle: (" << x_left << ", " << y_top << ") to (" << x_right << ", " << y_bottom << ")" << endl;

            }
            if (match == 10 && num == 3)
            {
                if (!rectReady)
                {
                    x_left = 150;  // Fixed square coordinates (top-left corner)
                    y_top = 150;
                    x_right = 350; // Side length of 200 (150 + 200)
                    y_bottom = 350;
                    Rectangle(hdc, x_left, y_top, x_right, y_bottom); // Draw the square
                    rectReady = true; // Mark square as drawn
                }

                COLORREF unclippedColor = RGB(255, 255, 255); // Gray color for unclipped portion
                DDALine(hdc, X1, Y1, x2, y2, unclippedColor);

                CohenSuth(hdc, X1, Y1, x2, y2, x_left, y_top, x_right, y_bottom, Y);

                cout << "Line drawn from (" << X1 << ", " << Y1 << ") to (" << x2 << ", " << y2 << ")" << endl;
                cout << "Clipping square: (" << x_left << ", " << y_top << ") to (" << x_right << ", " << y_bottom << ")" << endl;
            }


            if (match == 12)
            {
                /*Ellipse(hdc, xc, yc, x2, y2, Y);
                if (nume == 1) {
                    FillingL(hdc, xc, yc, Y, Y, 1, R);
                }
                else if (nume == 2) {
                    FillingL(hdc, xc, yc, Y, Y, 2, R);
                }
                else if (nume == 3) {
                    FillingL(hdc, xc, yc, Y, Y, 3, R);
                }
                else if (nume == 4) {
                    FillingL(hdc, xc, yc, Y, Y, 4, R);
                }
                else if (nume == 5) {
                    FillingL(hdc, xc, yc, Y, Y, 5, R);
                }
                else if (nume == 6) {
                    RFloodFill(hdc, xc, yc, Y, Y, 6);
                }
                else if (nume == 7) {
                    NRFloodFill(hdc, xc, yc, Y, Y, 7);
                }*/
                Ellipse(hdc, X1, Y1, abs(x2 - X1), abs(y2 - Y1), Y);

                nume = 0;
                match = 0;
            }
            if (match == 13)
            {
                DrawEllipsePolar(hdc, X1, Y1, abs(x2 - X1), abs(y2 - Y1), Y);

                
                nume = 0;
                match = 0;
            }
            if (match == 14)
            {

                DrawEllipsePolar(hdc, X1, Y1, abs(x2 - X1), abs(y2 - Y1), Y);


               /* midptellipse(hdc, xc, yc, x2, y2, Y);
                if (nume == 1) {
                    FillingL(hdc, xc, yc, Y, Y, 1, R);
                }
                else if (nume == 2) {
                    FillingL(hdc, xc, yc, Y, Y, 2, R);
                }
                else if (nume == 3) {
                    FillingL(hdc, xc, yc, Y, Y, 3, R);
                }
                else if (nume == 4) {
                    FillingL(hdc, xc, yc, Y, Y, 4, R);
                }
                else if (nume == 5) {
                    FillingL(hdc, xc, yc, Y, Y, 5, R);
                }
                else if (nume == 6) {
                    RFloodFill(hdc, xc, yc, Y, Y, 6);
                }
                else if (nume == 7) {
                    NRFloodFill(hdc, xc, yc, Y, Y, 7);
                }*/
                nume = 0;
                match = 0;
            }




            if ((match == 15 || match == 16 || match == 26) && Num_of_Points == 0)
            {
                P[0].x = LOWORD(lParam);
                P[0].y = HIWORD(lParam);
                Num_of_Points++;
            }
            else if ((match == 15 || match == 16 || match == 26) && Num_of_Points == 1)
            {
                P[1].x = LOWORD(lParam);
                P[1].y = HIWORD(lParam);
                Num_of_Points++;
            }
            else if ((match == 15 || match == 16 || match == 26) && Num_of_Points == 2)
            {
                P[2].x = LOWORD(lParam);
                P[2].y = HIWORD(lParam);
                Num_of_Points++;
            }
            else if ((match == 15 || match == 16 || match == 26) && Num_of_Points == 3)
            {
                P[3].x = LOWORD(lParam);
                P[3].y = HIWORD(lParam);
                Num_of_Points++;
            }
            else if ((match == 15 || match == 16 || match == 26) && Num_of_Points == 4)
            {
                P[4].x = LOWORD(lParam);
                P[4].y = HIWORD(lParam);
                Num_of_Points++; // Now Num_of_Points == 5
                if (match == 16 && num == 1 ) // Rectangle clipping mode
                {
                    bool allInside = true;
                    for (int i = 0; i < 5; i++)
                    {
                        if (P[i].x < x_left || P[i].x > x_right || P[i].y < y_bottom || P[i].y > y_top)
                        {
                            allInside = false;
                            break;
                        }
                    }
                    if (allInside)
                    {
                        SelectObject(hdc, GetStockObject(DC_PEN));
                        SetDCPenColor(hdc, Y);
                        Polygon(hdc, P, 5);
                        cout << "All points inside the rectangle, drawing in selected color.\n";
                    }
                    else
                    {
                        COLORREF unclippedColor = RGB(255, 255, 255);
                        SelectObject(hdc, GetStockObject(DC_PEN));
                        SetDCPenColor(hdc, unclippedColor);
                        Polygon(hdc, P, 5);
                        SetDCPenColor(hdc, Y);
                        PolygonClip(hdc, P, 5, x_left, y_top, x_right, y_bottom, Y);
                        cout << "Some points outside, clipping applied.\n";
                    }
                    Num_of_Points = 0; // Reset after clipping and drawing
                }


                if (match == 16 && num == 1 ) // Rectangle clipping mode
                {
                    bool allInside = true;
                    for (int i = 0; i < 5; i++)
                    {
                        if (P[i].x < x_left || P[i].x > x_right || P[i].y < y_bottom || P[i].y > y_top)
                        {
                            allInside = false;
                            break;
                        }
                    }
                    if (allInside)
                    {
                        SelectObject(hdc, GetStockObject(DC_PEN));
                        SetDCPenColor(hdc, Y);
                        Polygon(hdc, P, 5);
                        cout << "All points inside the rectangle, drawing in selected color.\n";
                    }
                    else
                    {
                        COLORREF unclippedColor = RGB(255, 255, 255);
                        SelectObject(hdc, GetStockObject(DC_PEN));
                        SetDCPenColor(hdc, unclippedColor);
                        Polygon(hdc, P, 5);
                        SetDCPenColor(hdc, Y);
                        PolygonClip(hdc, P, 5, x_left, y_top, x_right, y_bottom, Y);
                        cout << "Some points outside, clipping applied.\n";
                    }
                    Num_of_Points = 0; // Reset after clipping and drawing
                }

                if (match == 16 && num == 3)
                {
                    // Draw the square once
                    if (!rectReady)
                    {
                        x_left = 150;
                        y_top = 150;
                        x_right = 350;
                        y_bottom = 350;
                        Rectangle(hdc, x_left, y_top, x_right, y_bottom);  // or DrawSquare(...)
                        rectReady = true;
                    }

                    if (Num_of_Points < 5)  // or however many points you want
                    {
                        P[Num_of_Points].x = LOWORD(lParam);
                        P[Num_of_Points].y = HIWORD(lParam);
                        SetPixel(hdc, P[Num_of_Points].x, P[Num_of_Points].y, Y);
                        Num_of_Points++;
                    }
                    if (Num_of_Points == 5)
                    {
                        PolygonClip(hdc, P, 5, x_left, y_top, x_right, y_bottom, Y);
                        Num_of_Points = 0;
                    }
                }

                if (match == 16 && num == 1 ) // Rectangle clipping mode
                {
                    bool allInside = true;
                    for (int i = 0; i < 5; i++)
                    {
                        if (P[i].x < x_left || P[i].x > x_right || P[i].y < y_bottom || P[i].y > y_top)
                        {
                            allInside = false;
                            break;
                        }
                    }
                    if (allInside)
                    {
                        SelectObject(hdc, GetStockObject(DC_PEN));
                        SetDCPenColor(hdc, Y);
                        Polygon(hdc, P, 5);
                        cout << "All points inside the rectangle, drawing in selected color.\n";
                    }
                    else
                    {
                        COLORREF unclippedColor = RGB(255, 255, 255);
                        SelectObject(hdc, GetStockObject(DC_PEN));
                        SetDCPenColor(hdc, unclippedColor);
                        Polygon(hdc, P, 5);
                        SetDCPenColor(hdc, Y);
                        PolygonClip(hdc, P, 5, x_left, y_top, x_right, y_bottom, Y);
                        cout << "Some points outside, clipping applied.\n";
                    }
                    Num_of_Points = 0; // Reset after clipping and drawing
                }
                else if (match == 15 || match == 26)
                {
                    // Draw the polygon outline after the 5th click
                    SelectObject(hdc, GetStockObject(DC_PEN));
                    SetDCPenColor(hdc, Y);
                    Polygon(hdc, P, 5);
                    cout << "Polygon outline drawn with 5 points. Click again to fill.\n";
                    // Do NOT reset Num_of_Points here; wait for the 6th click to fill
                }
            }
            else if ((match == 15 || match == 26) && Num_of_Points == 5)
            {
                // 6th click triggers the fill
                bool pointsInBounds = true;
                for (int i = 0; i < 5; i++)
                {
                    if (P[i].x < 0 || P[i].x >= 900 || P[i].y < 0 || P[i].y >= 600)
                    {
                        pointsInBounds = false;
                        cout << "Point P[" << i << "] (" << P[i].x << ", " << P[i].y << ") is out of bounds. Adjust points to fit within (0,0) to (899,599).\n";
                        break;
                    }
                }
                if (!pointsInBounds)
                {
                    cout << "Skipping fill due to out-of-bounds points.\n";
                    Num_of_Points = 0;
                    return 0;
                }
                if (match == 15)
                {
                    ConvexFill(hdc, P, 5, Y);
                    cout << "Convex fill applied to polygon with color RGB("
                        << GetRValue(Y) << ", " << GetGValue(Y) << ", " << GetBValue(Y) << ").\n";
                }
                else if (match == 26)
                {
                    ConvexFill(hdc, P, 5, Y);
                    cout << "General fill applied to polygon with color RGB("
                        << GetRValue(Y) << ", " << GetGValue(Y) << ", " << GetBValue(Y) << ").\n";
                }
                Num_of_Points = 0; // Reset to allow new polygon after filling
            }
          /*  if (match == 22) {

                    int x = LOWORD(lParam);
                    int y = HIWORD(lParam);
                    points1[clickCount++] = Point(x, y);

                    SetPixel(hdc, x, y, RGB(0, 0, 255));

                    if (clickCount == 6) {
                        cout << "Clicked: " << clickCount << " at (" << x << ", " << y << ")" << endl;
                        cardinalSpline(hdc, points1, 6, 0.4);
                        clickCount = 0;
                    }
                    break;
            }*/

            if ((match == 23) && counter == 0)
            {
                s[counter] = Vector(LOWORD(lParam), HIWORD(lParam));
                counter++;
            }
            else if ((match == 23) && counter == 1)
            {
                s[counter] = Vector(LOWORD(lParam), HIWORD(lParam));
                DrawSquare(hdc, s[0][0], s[0][1], s[1][0], s[1][1], Y);
                counter++;
            }
            else if ((match == 23) && counter == 2)
            {
                fillverticalwithhermit(hdc, x_left, x_right, y_top, y_bottom, Y);
                counter = 0;
            }
            else if ((match == 24) && counter5 == 0)
            {
                s[counter5] = Vector(LOWORD(lParam), HIWORD(lParam));
                counter5++;
            }
            else if ((match == 24) && counter5 == 1)
            {
                s[counter5] = Vector(LOWORD(lParam), HIWORD(lParam));
                Drawrectangley(hdc, s[0][0], s[0][1], s[1][0], s[1][1], Y);
                counter5++;
            }
            else if ((match == 24) && counter5 == 2)
            {
                fillhorizontalwithbezeir(hdc, x_left, x_right, y_top, y_bottom, Y);
                counter5 = 0;
            }
            break;

    case WM_DESTROY:
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
        }
        return 0;
    }
    void AddMenus(HWND hwnd)
    {

        hmenu = CreateMenu();

        HMENU hcolor = CreateMenu();
        HMENU Save = CreateMenu();
        HMENU circle = CreateMenu();
        HMENU line = CreateMenu();
        HMENU FILL = CreateMenu();
        HMENU fillbycircle = CreateMenu();
        HMENU cleare = CreateMenu();
        HMENU Ellips = CreateMenu();
        HMENU clipping = CreateMenu();
        HMENU Polygon = CreateMenu();
        HMENU FloodFill = CreateMenu();
        HMENU Curve = CreateMenu();
        HMENU curvefilling = CreateMenu();


        AppendMenu(hcolor, MF_STRING, ID_COLOR_RED, L"red");
        AppendMenu(hcolor, MF_STRING, ID_COLOR_BLUE, L"blue");
        AppendMenu(hcolor, MF_STRING, ID_COLOR_GREEN, L"green");
        AppendMenu(hcolor, MF_STRING, ID_COLOR_YELLOW, L"yellow");
        AppendMenu(hcolor, MF_STRING, ID_COLOR_ORANGE, L"orange");
        AppendMenu(hcolor, MF_STRING, ID_COLOR_BLACK, L"black");
        AppendMenu(hcolor, MF_STRING, ID_COLOR_PURPLE, L"purple");
        AppendMenu(hcolor, MF_STRING, ID_COLOR_GREY, L"grey");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hcolor, L"Color");

        AppendMenu(circle, MF_STRING, ID_CIRCLE_POLAR, L"Polar");
        AppendMenu(circle, MF_STRING, ID_CIRCLE_DIRECT, L"Direct");
        AppendMenu(circle, MF_STRING, ID_CIRCLE_ITERATIVE, L"Iterative");
        AppendMenu(circle, MF_STRING, ID_CIRCLE_MIDPOINT, L"Midpoint");
        AppendMenu(circle, MF_STRING, ID_CIRCLE_MODIFIED, L"Modified MidPoint");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)circle, L"Circle");

        AppendMenu(line, MF_STRING, ID_LINE_DDA, L"DDA");
        AppendMenu(line, MF_STRING, ID_LINE_MIDPOINT, L"MidPoint");
        AppendMenu(line, MF_STRING, ID_LINE_PARAMETRIC, L"Parametric");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)line, L"Line");

        AppendMenu(Ellips, MF_STRING, ID_ELLIPSE_DIRECT, L"Direct Ellipse");
        AppendMenu(Ellips, MF_STRING, ID_ELLIPSE_POLAR, L"Polar Ellipse");
        AppendMenu(Ellips, MF_STRING, ID_ELLIPSE_MID, L"Midpoint Ellipse");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)Ellips, L"Ellipse");

        AppendMenu(FILL, MF_STRING, ID_FILL_Q1, L"First Quarter");
        AppendMenu(FILL, MF_STRING, ID_FILL_Q2, L"Second Quarter");
        AppendMenu(FILL, MF_STRING, ID_FILL_Q3, L"Third Quarter");
        AppendMenu(FILL, MF_STRING, ID_FILL_Q4, L"Fourth Quarter");
        AppendMenu(FILL, MF_STRING, ID_FILL_FULL, L"Full Circle");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)FILL, L"Filling");

        AppendMenu(fillbycircle, MF_STRING, ID_CIRCLE_Q1, L"First Quarter");
        AppendMenu(fillbycircle, MF_STRING, ID_CIRCLE_Q2, L"Second Quarter");
        AppendMenu(fillbycircle, MF_STRING, ID_CIRCLE_Q3, L"Third Quarter");
        AppendMenu(fillbycircle, MF_STRING, ID_CIRCLE_Q4, L"Fourth Quarter");
        AppendMenu(fillbycircle, MF_STRING, ID_CIRCLE_FULL, L"Full Circle");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)fillbycircle, L"Filling by Circle");

        AppendMenu(FloodFill, MF_STRING, ID_FILL_RECURSIVE, L"Recursive");
        AppendMenu(FloodFill, MF_STRING, ID_FILL_NONRECURSIVE, L"Non-Recursive");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)FloodFill, L"Flood Fill");

        AppendMenu(curvefilling, MF_STRING, ID_FILL_SQUARE_CURVE, L"Fill Square with Curve");
        AppendMenu(curvefilling, MF_STRING, ID_FILL_RECT_BEZIER, L"Fill Rectangle with Bezier");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)curvefilling, L"Filling by Curve");

        AppendMenu(Curve, MF_STRING, ID_CARDINAL, L"Cardinal Spline Curve");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)Curve, L"Curve");

        AppendMenu(Polygon, MF_STRING, ID_POLYGON_CONVEX, L"Convex");
        AppendMenu(Polygon, MF_STRING, ID_POLYGON_NONCONVEX, L"Non-Convex");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)Polygon, L"Polygon");

        AppendMenu(clipping, MF_STRING, ID_CLIP_RECT, L"Rectangle Clipping Window");
        AppendMenu(clipping, MF_STRING, ID_CLIP_SQUARE, L"Square Clipping Window");
        AppendMenu(clipping, MF_STRING, ID_CLIP_LINE, L"Line");
        AppendMenu(clipping, MF_STRING, ID_CLIP_POINT, L"Point");
        AppendMenu(clipping, MF_STRING, ID_CLIP_POLYGON, L"Polygon");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)clipping, L"Clipping");

        AppendMenu(hmenu, MF_STRING, ID_INPUT_LINE, L"Draw Line From Input");

        AppendMenu(Save, MF_STRING, ID_SAVE, L"Save");
        AppendMenu(Save, MF_STRING, ID_LOAD, L"Load");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)Save, L"Save & Load");

        AppendMenu(cleare, MF_STRING, ID_CLEAR_SCREEN, L"Clear");
        AppendMenu(hmenu, MF_POPUP, (UINT_PTR)cleare, L"Clear Screen");


        SetMenu(hwnd, hmenu);
    }
