using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class fireHP : MonoBehaviour
{
    public float hp = 1000;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(hp < 0)
        {
            GameObject.Destroy(this.gameObject);
        }
    }
}
