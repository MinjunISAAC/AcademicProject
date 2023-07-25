using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Ardunity;
using UnityEngine.UI;
public class sigleButton : MonoBehaviour
{
    private RaycastHit hit;

    public GameObject collObj;

    public fireCheck collObjValue;

    public AudioSource theWindow;
    [SerializeField] private AudioClip[] clip2;

    public GameObject windowObj1;
    public GameObject windowObj2;
    public GameObject windowObj3;
    public GameObject windowObj4;
    public GameObject windowObj5;
    public GameObject windowObj6;
    public GameObject windowObj7;
    public GameObject windowObj8;

    public windowWork windowObjCheck1;
    public windowWork windowObjCheck2;
    public windowWork windowObjCheck3;
    public windowWork windowObjCheck4;
    public windowWork windowObjCheck5;
    public windowWork windowObjCheck6;
    public windowWork windowObjCheck7;
    public windowWork windowObjCheck8;

    public float maxDistance = 300f;

    public DigitalInput buttonValue;
    public DigitalInput buttonValue2;



    public GameObject fireChecker;

    public objectUsing fireCheckImage;
    public Image fireExImage;
    public Sprite fireExImageOn;
    public Sprite fireExImageOff;
    public ParticleSystem ps;
    
    float speed = 1.0f;
    Vector3 target1 = new Vector3(11.85f, 2.275f, -8.475f);
    Vector3 target2 = new Vector3(11.85f, 2.275f, -11.475f);
    Vector3 target3 = new Vector3(11.85f, 2.275f, -15.53f);
    Vector3 target4 = new Vector3(11.85f, 2.275f, -18.53f);
    Vector3 target5 = new Vector3(11.85f, 2.275f, -22.521f);
    Vector3 target6 = new Vector3(11.85f, 2.275f, -25.521f);
    Vector3 target7 = new Vector3(11.85f, 2.275f, -29.505f);
    Vector3 target8 = new Vector3(11.85f, 2.275f, -32.505f);
    public Vector3 fireExPos;

    void Start()
    {
        buttonValue = GameObject.Find("ClickButton2").GetComponent("DigitalInput") as DigitalInput;
        buttonValue2 = GameObject.Find("ClickButton").GetComponent("DigitalInput") as DigitalInput;

        fireCheckImage = GameObject.Find("fireExCheck").GetComponent("objectUsing") as objectUsing;

        collObjValue = GameObject.Find("fire extinguisher").GetComponent("fireCheck") as fireCheck;

        theWindow = GetComponent<AudioSource>();

        windowObj1 = GameObject.Find("WindowUp1");
        windowObj2 = GameObject.Find("WindowUp3");
        windowObj3 = GameObject.Find("WindowUp5");
        windowObj4 = GameObject.Find("WindowUp7");
        windowObj5 = GameObject.Find("WindowUp9");
        windowObj6 = GameObject.Find("WindowUp11");
        windowObj7 = GameObject.Find("WindowUp13");
        windowObj8 = GameObject.Find("WindowUp15");

        windowObjCheck1 = GameObject.Find("WindowUp1").GetComponent("windowWork") as windowWork;
        windowObjCheck2 = GameObject.Find("WindowUp3").GetComponent("windowWork") as windowWork;
        windowObjCheck3 = GameObject.Find("WindowUp5").GetComponent("windowWork") as windowWork;
        windowObjCheck4 = GameObject.Find("WindowUp7").GetComponent("windowWork") as windowWork;
        windowObjCheck5 = GameObject.Find("WindowUp9").GetComponent("windowWork") as windowWork;
        windowObjCheck6 = GameObject.Find("WindowUp11").GetComponent("windowWork") as windowWork;
        windowObjCheck7 = GameObject.Find("WindowUp13").GetComponent("windowWork") as windowWork;
        windowObjCheck8 = GameObject.Find("WindowUp15").GetComponent("windowWork") as windowWork;
    }

    // Update is called once per frame
    void Update()
    {
        Debug.Log(fireCheckImage.check);
        if(buttonValue.Value == true && buttonValue2.Value == true)
        {
            collObjValue.check1 = false;
            collObj.transform.parent = null;
            fireExImage.sprite = fireExImageOff;
            collObj.transform.position = new Vector3(10.95953f, 0, -5.610753f);
            collObj.transform.rotation = Quaternion.Euler(new Vector3(-90f, 0, 80.19f));

            ps.Stop();
        }
        if (Input.GetMouseButton(0) == true && Input.GetMouseButton(1) == true && collObjValue.check1 == true)
        {
            collObjValue.check1 = false;
            fireExImage.sprite = fireExImageOff;
            collObj.transform.parent = null;
            collObj.transform.position = fireExPos;
            collObj.transform.position = new Vector3(10.95953f, 0, -5.610753f);
            collObj.transform.rotation = Quaternion.Euler(new Vector3(-90f, 0, 80.19f));

            ps.Stop();
        }



        if (Physics.Raycast(transform.position, transform.forward, out hit, 5)) { 
            Debug.Log("hit point : " + hit.point + ", distance : " + hit.distance + ", name : " + hit.collider.name); 
            Debug.DrawRay(transform.position, transform.forward * hit.distance, Color.red);


            if(hit.collider.gameObject == collObj && (buttonValue.Value == true || Input.GetMouseButton(1)))
            {
                fireMe();
                Debug.Log("여기야!!!");
                fireCheckImage.check = true;
                collObjValue.check1 = true;
            }

            
            
            if(hit.collider.gameObject == windowObj1 && (buttonValue.Value == true || Input.GetMouseButton(1)))
            {
                windowObjCheck1.windowCh = true;
                soundSet(0);

            }
            if (hit.collider.gameObject == windowObj2 && (buttonValue.Value == true || Input.GetMouseButton(1)))
            {
                windowObjCheck2.windowCh = true;
                soundSet(0);

            }
            if (hit.collider.gameObject == windowObj3 && (buttonValue.Value == true || Input.GetMouseButton(1)))
            {
                windowObjCheck3.windowCh = true;
                soundSet(0);

            }
            if (hit.collider.gameObject == windowObj4 && (buttonValue.Value == true || Input.GetMouseButton(1)))
            {
                windowObjCheck4.windowCh = true;
                soundSet(0);

            }
            if (hit.collider.gameObject == windowObj5 && (buttonValue.Value == true || Input.GetMouseButton(1)))
            {
                windowObjCheck5.windowCh = true;
                soundSet(0);

            }
            if (hit.collider.gameObject == windowObj6 && (buttonValue.Value == true || Input.GetMouseButton(1)))
            {
                windowObjCheck6.windowCh = true;
                soundSet(0);

            }
            if (hit.collider.gameObject == windowObj7 && (buttonValue.Value == true || Input.GetMouseButton(1)))
            {
                windowObjCheck7.windowCh = true;
                soundSet(0);

            }
            if (hit.collider.gameObject == windowObj8 && (buttonValue.Value == true || Input.GetMouseButton(1)))
            {
                windowObjCheck8.windowCh = true;
                soundSet(0);

            }
        } 
        else { 
            Debug.DrawRay(transform.position, transform.forward * 1000f, Color.green); 
        }

        if (windowObjCheck1.windowCh == true)
        {
            windowObj1.transform.position = Vector3.Lerp(windowObj1.transform.position, target1, speed * Time.deltaTime);
            windowObjCheck1.windowOpenCheck = true;
        }
        if (windowObjCheck2.windowCh == true) 
        {
            windowObj2.transform.position = Vector3.Lerp(windowObj2.transform.position, target2, speed * Time.deltaTime);
            windowObjCheck2.windowOpenCheck = true;
        }
        if (windowObjCheck3.windowCh == true)
        {
            windowObj3.transform.position = Vector3.Lerp(windowObj3.transform.position, target3, speed * Time.deltaTime);
            windowObjCheck3.windowOpenCheck = true;
        }
        if (windowObjCheck4.windowCh == true)
        {
            windowObj4.transform.position = Vector3.Lerp(windowObj4.transform.position, target4, speed * Time.deltaTime);
            windowObjCheck4.windowOpenCheck = true;
        }
        if (windowObjCheck5.windowCh == true)
        {
            windowObj5.transform.position = Vector3.Lerp(windowObj5.transform.position, target5, speed * Time.deltaTime);
            windowObjCheck5.windowOpenCheck = true;
        }
        if (windowObjCheck6.windowCh == true)
        {
            windowObj6.transform.position = Vector3.Lerp(windowObj6.transform.position, target6, speed * Time.deltaTime);
            windowObjCheck6.windowOpenCheck = true;
        }
        if (windowObjCheck7.windowCh == true)
        {
            windowObj7.transform.position = Vector3.Lerp(windowObj7.transform.position, target7, speed * Time.deltaTime);
            windowObjCheck7.windowOpenCheck = true;
        }
        if (windowObjCheck8.windowCh == true)
        {
            windowObj8.transform.position = Vector3.Lerp(windowObj8.transform.position, target8, speed * Time.deltaTime);
            windowObjCheck8.windowOpenCheck = true;
        }






        Debug.Log(fireCheckImage.check);

        fireStart();
    }

    public void fireMe()
    {
        collObj.transform.position = new Vector3(transform.position.x, transform.position.y-1, transform.position.z-2);
        collObj.transform.parent = transform;

        fireExImage.sprite = fireExImageOn;
    }

    public void fireDrop()
    {
        collObj.transform.parent = null;
        collObj.transform.position = new Vector3(collObj.transform.position.x, 0, collObj.transform.position.z);
    }

    public void fireStart()
    {
        if((buttonValue.Value == true || Input.GetMouseButton(1)) && collObjValue.check1 == true) {
            ps.Play();
            Debug.Log("성공");
        }
        else
        {
            ps.Stop();
        }
    }

    public void soundSet(int i)
    {
        theWindow.clip = clip2[i];
        theWindow.Play();
    }

}
