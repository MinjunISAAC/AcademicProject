using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class particleColl : MonoBehaviour
{
    public float hp = 1000;
    public ParticleSystem firePs;

    void Update()
    {
        Debug.Log(hp);
        fireScale();
    }

    private void OnParticleCollision(GameObject other)
    {
        Debug.Log("fire 파티클 충돌");
        hp -= 3;
    }

    void fireScale()
    {
        if(hp <= 900 && hp > 800)
        {
            firePs.transform.localScale = new Vector3(0.9f, 0.9f, 0.9f);
        }
        else if (hp <= 800 && hp > 700)
        {
            firePs.transform.localScale = new Vector3(0.8f, 0.8f, 0.8f);
        }
        else if(hp <= 700 && hp > 600)
        {
            firePs.transform.localScale = new Vector3(0.7f, 0.7f, 0.7f);
        }
        else if (hp <= 600 && hp > 500)
        {
            firePs.transform.localScale = new Vector3(0.6f, 0.6f, 0.6f);
        }
        else if (hp <= 500 && hp > 400)
        {
            firePs.transform.localScale = new Vector3(0.5f, 0.5f, 0.5f);
        }
        else if (hp <= 400 && hp > 300)
        {
            firePs.transform.localScale = new Vector3(0.4f, 0.4f, 0.4f);
        }
        else if (hp <= 300 && hp > 200)
        {
            firePs.transform.localScale = new Vector3(0.3f, 0.3f, 0.3f);
        }
        else if (hp <= 200 && hp > 100)
        {
            firePs.transform.localScale = new Vector3(0.2f, 0.2f, 0.2f);
        }
        else if (hp <= 100 && hp >= 0)
        {
            firePs.transform.localScale = new Vector3(0.1f, 0.1f, 0.1f);
        }
        else if(hp < 0)
        {
            Destroy(gameObject);
            Debug.Log("삭제");
        }
    }
}
