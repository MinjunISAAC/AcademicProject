using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using Ardunity;

public class PlayerController : MonoBehaviour
{
    private MoveMent3D movement3D;
    
    public AnalogInput joystick_x;
    public AnalogInput joystick_y;

    public DigitalInput btn1;
    public DigitalInput btn2;
    public bool isJumping;

    public Text tex1;
    public Text tex2;
    public Text tex3;
    public Text tex4;
    private float moveHorizeontal;
    private float moveVertical;

    public bool blueToothe;
    
    private float moveSpeed;
    private Rigidbody rigidbody3D;
    private void Start()
    {
        blueToothe = false;
        joystick_x = GameObject.Find("HorizontalAxis").GetComponent("AnalogInput") as AnalogInput;
        joystick_y = GameObject.Find("VerticalAxis").GetComponent("AnalogInput") as AnalogInput;
        btn1 = GameObject.Find("Button1").GetComponent("DigitalInput") as DigitalInput;
        btn2 = GameObject.Find("Button2").GetComponent("DigitalInput") as DigitalInput;

        rigidbody3D = GetComponent<Rigidbody>();
    }
    private void Awake()
    {
        movement3D = GetComponent<MoveMent3D>();
    }

    public void blue()
    {
        blueToothe = true;
    }
    private void Update()
    {
        float x = Input.GetAxis("Horizontal");
        float y = Input.GetAxis("Vertical");
        
        float xValue = 1.0f - joystick_x.Value;
        float yValue = joystick_y.Value;

        tex1.text = x.ToString();
        tex2.text = y.ToString();

        if(btn1.Value == true)
        {
            isJumping = true;
        }
        
        if (xValue > 0.55f)
        {
            moveHorizeontal = (xValue - 0.5f) * 2;
        }
        else if (xValue < 0.45f)
        {
            moveHorizeontal = (xValue - 0.5f) * 2;
        }
        else
        {
            moveHorizeontal = 0;
        }

        if (yValue > 0.55f)
        {
            moveVertical = (yValue - 0.5f) * 2;
        }
        else if (yValue < 0.45f)
        {
            moveVertical = (yValue - 0.5f) * 2;
        }
        else
        {
            moveVertical = 0;
        }

        tex3.text = moveHorizeontal.ToString();
        tex4.text = moveVertical.ToString();
        
        if(blueToothe == true)
        {
            if (moveHorizeontal != 0 || moveVertical != 0)
            {
                rigidbody3D.AddForce(moveHorizeontal * 5, 0, moveVertical * 5);
            }
        }
    }

    void FixeUpdate()
    {
        Jump();
    }

    void Jump()
    {
        if (!isJumping)
            return;
        rigidbody3D.MovePosition(transform.position + Vector3.up);
        isJumping = false;
    }
}
