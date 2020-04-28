#version 330 core
in vec3 PointPos;       //点的位置
in mat3 normalMat;      //法向量变换矩阵
in vec3 Point2Camera;   //点到相机距离
out vec4 FragColor;     //输出片段的颜色
uniform vec3 lightPos;  //光源位置

const float constant = 1.0;      //光线衰减常数系数
const float linear = 0.1;       //光线衰减线性系数
const float quadratic = 0.01;    //光线衰减二次系数

const float shininess = 10.0;    //高光系数
const float ambientCoef = 1.0;  //环境光强度
const float diffuseCoef = 0.7;  //漫反射强度
const float specularCoef = 0.4; //镜面反射强度

vec3 ambient = vec3(0.3f, 0.3f, 0.3f);  //环境光
vec3 diffuse = vec3(0.7f, 0.7f, 0.7f);  //漫反射光
vec3 specular = vec3(1.0f, 1.0f, 1.0f); //镜面反射光
vec4 color = vec4(0.0, 1.0, 1.0, 1.0);  //原来颜色


vec3 calNormal(vec2 pointcoord)
{
    vec3 xcoord = vec3(1.0, 0.0, 0.0);
    vec3 ycoord = vec3(0.0, 1.0, 0.0);
    vec3 zcoord = normalize(Point2Camera);
    vec3 normal = pointcoord.x * xcoord + pointcoord.y * ycoord + zcoord;
    normal = normalize(normalMat * normal);
    return normal;
}


void main()
{
    //原坐标范围为x[0,1],y[0,1]->现坐标范围x[-0.5,0.5],y[-0.5,0.5]
    vec2 pointcoord = gl_PointCoord - vec2(0.5, 0.5);
    //距离原点超过0.5的部分裁掉
    if (length(pointcoord) > 0.5)
        discard;

    vec3 normal= calNormal(pointcoord);
    //计算出片段对应于球面上的位置
    vec3 FragPos = PointPos + normal;

    //计算光线方向
    float lightDist = distance(FragPos, lightPos);
    vec3 lightDir = normalize(lightPos - FragPos);
    //计算法向量和入射光线向量的内积diff
    float diff = max(dot(normal, lightDir), 0.0);
    //计算观察方向向量和镜面反射光线向量的内积spec
    float spec = pow(max(dot(normalize(Point2Camera + PointPos - FragPos), reflect(-lightDir, normal)), 0.0), shininess);
    //计算衰减系数
    float attnuation = 1.0 / (constant + linear * lightDist + quadratic * pow(lightDist, 2));
    //将环境光分量、漫反射分量和镜面反射分量相加，再与原颜色相乘得到最终片段的颜色
    FragColor = vec4(ambientCoef * ambient + attnuation * (diffuseCoef * diff * diffuse + specularCoef * spec * specular), 1.0) * color;
}
