using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneSwapper : MonoBehaviour
{
    public int currentScene;
    
	// Update is called once per frame
	void Update ()
    {
        if (Input.GetKeyDown(KeyCode.LeftArrow))
        {
            int newScene = 0;
            if (currentScene == 0)
                newScene = 9;
            else
                newScene = currentScene - 1;

            SceneManager.LoadScene(newScene);
        }

        if (Input.GetKeyDown(KeyCode.RightArrow))
        {
            int newScene = 0;
            if (currentScene == 9)
                newScene = 0;
            else
                newScene = currentScene + 1;

            SceneManager.LoadScene(newScene);
        }
    }
}
