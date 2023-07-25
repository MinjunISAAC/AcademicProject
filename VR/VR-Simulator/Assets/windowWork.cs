using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class windowWork : MonoBehaviour
{
    public bool windowCh;
    public bool windowOpenCheck;

    float speed = 1.0f;
    Vector3 target = new Vector3(11.8f, 2.2f, -8.475f);

    void Start()
    {
        windowCh = false;
        windowOpenCheck = false;
    }

    void Update()
    {
    }

    public void windowOpen()
    {
        transform.position = Vector3.Lerp(transform.position, target, speed * Time.deltaTime);
        windowOpenCheck = true;
    }

    public void windowCheck()
    {
        windowCh = true;
    }

}
