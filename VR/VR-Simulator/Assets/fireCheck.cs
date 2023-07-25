using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using Ardunity;

public class fireCheck : MonoBehaviour
{

    public GameObject player;
    public Camera cam;
    public bool check1 = false;

    public GameObject fireChecker;
    public Image fireExImage;
    public Sprite fireExImageOn;

    public DigitalInput button2;

    // Start is called before the first frame update
    void Start()
    {
        button2 = GameObject.Find("ClickButton2").GetComponent("DigitalInput") as DigitalInput;
    }

    // Update is called once per frame
    void Update()
    {
        if (check1 == true)
        {
            fireMe();
        }

    }

    public void fireMe()
    {
        this.transform.position = new Vector3(cam.transform.position.x, cam.transform.position.y, cam.transform.position.z);
        this.transform.parent = cam.transform;
        fireExImage.sprite = fireExImageOn;
    }

    public void check()
    {
        check1 = true;
    }
}
